#ifndef FILTERMEDIAN_H
#define FILTERMEDIAN_H

#include "filter.h"
class FilterMedian : public Filter
{
public:
    FilterMedian(int xLo, int yLo, int xHi, int yHi);
    ~FilterMedian();
    virtual std::vector<BGRA> applyFilter(Canvas2D *canvas);
    std::vector<BGRA> readFloats(std::vector<float> reds, std::vector<float> greens, std::vector<float> blues);

    std::vector<float> convolve(int kernelR, int kernelC, std::vector<float> in, int width, int height);
};

#endif // FILTERMEDIAN_H
