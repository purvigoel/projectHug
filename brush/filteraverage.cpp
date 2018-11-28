#include "filteraverage.h"
#include <iostream>
#include "math.h"
#include "Settings.h"
FilterAverage::FilterAverage(int xLo, int yLo, int xHi, int yHi)
: Filter(xLo, yLo, xHi, yHi)
{
    m_filter = std::vector<float>{1};
    createFilter(settings.blurRadius);
}

FilterAverage::~FilterAverage()
{

}

void FilterAverage::createFilter(int radius){
    int filterSize = (2 * radius + 1);
    m_filter.clear();
    float count = 0;
    for(int i = 0; i < filterSize; i ++){
            float val = gaussian(i - radius, 0, 5.5);
            m_filter.push_back(val);
            count += val;
    }

    for(int i = 0; i < m_filter.size() ; i ++){
       m_filter[i] /= count;
    }
}

float FilterAverage::gaussian(int x, int y, float sigma){
    float coeff = 1/(sigma * sqrt(2 * M_PI));
    float e = exp(-1 * (float(x * x) + float(y * y))/ ( 2 * sigma * sigma));
    return coeff * e;
}

std::vector<BGRA> FilterAverage::applyFilter(Canvas2D * canvas){
    std::vector<float> reds(canvas->width() * canvas->height());
    std::vector<float> greens(canvas->width() * canvas->height());
    std::vector<float> blues(canvas->width() * canvas->height());
    BGRA* data = canvas->data();
    for(int i = 0; i < canvas->width() * canvas->height(); i++){
        reds[i] = (float(data[i].r));
        greens[i] = (float(data[i].g));
        blues[i] = (float(data[i].b));
    }

    int size = m_filter.size();
    std::vector<float> redsOut = Filter::convolve(m_filter, 1, size, reds, canvas->width(), canvas->height(),false, false);
    std::vector<float> bluesOut = Filter::convolve(m_filter, 1, size, blues, canvas->width(), canvas->height(),false, false);
    std::vector<float> greensOut = Filter::convolve(m_filter, 1, size, greens, canvas->width(), canvas->height(),false, false);

    //std::vector<BGRA> xDir = readFloats(redsOut, greensOut, bluesOut);

    std::vector<float> redsOutY = Filter::convolve(m_filter, size, 1, redsOut, canvas->width(), canvas->height(),false, false);
    std::vector<float> bluesOutY = Filter::convolve(m_filter, size, 1, bluesOut, canvas->width(), canvas->height(),false, false);
    std::vector<float> greensOutY = Filter::convolve(m_filter, size, 1, greensOut, canvas->width(), canvas->height(),false, false);

    return readFloats(redsOutY, greensOutY, bluesOutY);
}

std::vector<BGRA> FilterAverage::readFloats(std::vector<float> reds, std::vector<float> greens, std::vector<float> blues){
    std::vector<BGRA> out;
    for(int i =0 ; i <reds.size(); i++){
        BGRA col;
        col.r = reds[i];
        col.g = greens[i];
        col.b = blues[i];
        out.push_back(col);
    }
    return out;
}

