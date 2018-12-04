#ifndef FILTERAVERAGE_H
#define FILTERAVERAGE_H

#include "filter.h"
class FilterAverage : public Filter
{
public:
    FilterAverage(int xLo, int yLo, int xHi, int yHi);
    ~FilterAverage();
    virtual std::vector<BGRA> applyFilter(Canvas2D *canvas);
    std::vector<float> m_filter;
    void createFilter(int radius);
    std::vector<BGRA> readFloats(std::vector<float> reds, std::vector<float> greens, std::vector<float> blues);

    float gaussian(int x, int y, float sigma);
};

#endif // FILTERAVERAGE_H
