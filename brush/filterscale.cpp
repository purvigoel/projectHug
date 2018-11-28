#include "filterscale.h"
#include "math.h"
#include <iostream>
#include "Settings.h"
#include "Canvas2D.h"
FilterScale::FilterScale(int xLo, int yLo,int xHi,int yHi)
: Filter(xLo, yLo, xHi, yHi)
{

}

FilterScale::~FilterScale()
{

}


float FilterScale::triFilter(float point, float a){
   float radius = 1.0;
   if(a < 1){
       radius = 1.0/a;
   }
   if( ( point < -radius) || point > radius){
       return 0;
   }
   return (1-fabs(point)/radius) / radius;
}

BGRA FilterScale::hPrime(int k, float a, BGRA *data, int heightTillNow, int width){
    float sumR = 0;
    float weights_sum = 0;
    float sumG = 0;
    float sumB = 0;
    int left, right;
    float support;
    float center = float(k)/a + (( 1.0-a)/(2.0*a));

    support = (a > 1) ? 1 : 1/a;
    left = fmax(0.0,ceil(center - support));
    int startX = width * heightTillNow + center - left;
    right = fmin(float(width) - 1, floor(center + support));
    for(int i = left; i <= right; i++){
        float filterVal = triFilter(i-center, a);
        sumR += filterVal * data[startX + i].r;
        weights_sum += filterVal;
        sumB += filterVal * data[startX + i].b;
        sumG += filterVal * data[startX + i].g;
    }
    BGRA col;
    col.r = sumR/weights_sum;
    col.g = sumG/weights_sum;
    col.b = sumB/weights_sum;
    return col;
}

BGRA FilterScale::hPrimeY(int k, float a, BGRA *data, int widthTillNow, int width, int height){
    float sumR = 0;
    float weights_sum = 0;
    float sumG = 0;
    float sumB = 0;
    int low, high = 0;
    float support;
    float center = float(k)/a + (( 1.0-a)/(2.0*a));

    support = (a > 1) ? 1 : 1/a;
    low = fmax(0.0,ceil(center - support));
    int startY = width * (low) + widthTillNow;
    high = fmin(float(height) - 1, floor(center + support));

    for(int i = low; i <= high; i++){
        float filterVal = triFilter(i-center, a);
        sumR += filterVal * data[startY + (high - i) * width].r;
        weights_sum += filterVal;
        sumB += filterVal * data[startY + (high - i)  * width].b;
        sumG += filterVal * data[startY +(high - i)  * width].g;
    }
    BGRA col;
    col.r = sumR/weights_sum;
    col.g = sumG/weights_sum;
    col.b = sumB/weights_sum;
    return col;
}

std::vector<BGRA> FilterScale::scaleX(float scaleBy, float currentXLength, float currentYLength, Canvas2D * canvas){
    float newXLength = floor(currentXLength * scaleBy);
    scaleBy = newXLength / currentXLength;
    BGRA * data = canvas->data();
    std::vector<BGRA> newImage;
    for(int i = 0; i < currentYLength; i ++){
        for(int j = 0; j < newXLength; j ++){
            BGRA col = hPrime(j, scaleBy, data, i, currentXLength);
            newImage.push_back(col);
        }
    }
    canvas->resize(canvas->width() * scaleBy, canvas->height());
    return newImage;
}

std::vector<BGRA> FilterScale::scaleY(float scaleBy, float currentXLength, float currentYLength, Canvas2D * canvas){
    float newYLength = floor(currentYLength * scaleBy);
    scaleBy = newYLength / currentYLength;
    BGRA * data = canvas->data();
    std::vector<BGRA> newImage;
    for(int i = 0; i < newYLength; i ++){
        for(int j = 0; j < currentXLength; j ++){
            BGRA col = hPrimeY(i, scaleBy, data, j, currentXLength, currentYLength);
            newImage.push_back(col);
        }
    }
    canvas->resize(canvas->width(), canvas->height()* scaleBy);
    return newImage;
}

std::vector<BGRA> FilterScale::applyFilter(Canvas2D *canvas){
    cropMarqueeArea(canvas);
    std::vector<BGRA> retX = scaleX(settings.scaleX, canvas->width(), canvas->height(), canvas);
    paintCanvas(canvas, retX);
    std::vector<BGRA> ret = scaleY(settings.scaleY, canvas->width(), canvas->height(), canvas);
    paintCanvas(canvas, ret);
    return retX;
}

void FilterScale::cropMarqueeArea(Canvas2D * canvas){
    QPoint start = canvas->marqueeStart();
    QPoint end = canvas->marqueeStop();
    int xLo = start.x();
    int yLo = start.y();
    if(xLo < 0){
        xLo = 0;
    }
    if(yLo < 0){
        yLo = 0;
    }
    int xHi = end.x();
    int yHi = end.y();

    if(xHi < xLo){
        int temp = xLo;
        xLo = xHi;
        xHi = temp;
    }
    if(yHi < yLo){
        int temp = yLo;
        yLo = yHi;
        yHi = temp;
    }

    if(xHi > canvas->width()){
        xHi = canvas->width();
    }
    if(yHi > canvas->height()) {
        yHi = canvas->height();
    }

    if(xHi == xLo || yHi == yLo){
        xHi = canvas->width();
        xLo = 0;
        yHi = canvas->height();
        yLo = 0;

    }
    BGRA * data = canvas->data();
    int w = canvas->width();
    int h = canvas->height();
    std::vector<BGRA> tempPix;
    int pixNum = 0;
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j ++){
            if(i >= yLo && i < yHi && j >= xLo && j < xHi){
                tempPix.push_back(data[pixNum]);
            }
            pixNum += 1;
        }
    }
    canvas->resize(xHi - xLo , yHi - yLo );
    BGRA * newData = canvas->data();
    for(int i = 0; i < tempPix.size(); i ++){
        newData[i] = tempPix[i];
    }
}

void FilterScale::paintCanvas(Canvas2D * canvas, std::vector<BGRA> res){
    BGRA * data = canvas->data();
    for(int i = 0; i < res.size(); i ++){
        data[i] = res[i];
    }
}
