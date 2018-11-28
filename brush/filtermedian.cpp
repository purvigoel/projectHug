#include "filtermedian.h"
#include "math.h"
FilterMedian::FilterMedian(int xLo, int yLo, int xHi, int yHi)
: Filter(xLo, yLo, xHi, yHi)
{

}

FilterMedian::~FilterMedian()
{

}


std::vector<BGRA> FilterMedian::applyFilter(Canvas2D * canvas){
    std::vector<float> reds(canvas->width() * canvas->height());
    std::vector<float> greens(canvas->width() * canvas->height());
    std::vector<float> blues(canvas->width() * canvas->height());
    BGRA* data = canvas->data();
    for(int i = 0; i < canvas->width() * canvas->height(); i++){
        reds[i] = (float(data[i].r));
        greens[i] = (float(data[i].g));
        blues[i] = (float(data[i].b));
    }

    int size = int(5);
    std::vector<float> redsOut = convolve(size, size, reds, canvas->width(), canvas->height());
    std::vector<float> bluesOut = convolve(size, size, blues, canvas->width(), canvas->height());
    std::vector<float> greensOut = convolve(size, size, greens, canvas->width(), canvas->height());

    return readFloats(redsOut, greensOut, bluesOut);
}

std::vector<float> FilterMedian::convolve(int kernelR, int kernelC, std::vector<float> in, int width, int height){
    std::vector<float> result(width * height , 0.0);
    int radiusR = floor(kernelR/2);
    int radiusC = floor(kernelC / 2);

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = (r * width + c) ;

            std::vector<float> find_median;
            for( int ki = 0; ki < kernelR; ki ++){
                for(int kj = 0; kj < kernelC; kj ++){
                    float pixVal = 0.0;
                    if (!( (r + ki - radiusR) < 0 || (kj + c - radiusC) < 0 || (r + ki - radiusR) >= height || (kj + c - radiusC) >= width )){
                        int imageC = (r + ki - radiusR) * width + (kj + c - radiusC);
                        pixVal =  in[imageC];
                    }
                    find_median.push_back(pixVal);
                }
            }
            std::nth_element(find_median.begin(), find_median.begin() + find_median.size()/2, find_median.end());
            result[centerIndex] =  find_median[find_median.size()/2];
            }
        }
        return result;
}

std::vector<BGRA> FilterMedian::readFloats(std::vector<float> reds, std::vector<float> greens, std::vector<float> blues){
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
