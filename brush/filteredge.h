#ifndef FILTEREDGE_H
#define FILTEREDGE_H

#include "filter.h"
class FilterEdge : public Filter
{
public:
    FilterEdge(int xLo, int yLo, int xHi, int yHi);
    ~FilterEdge();
    virtual std::vector<BGRA> applyFilter(Canvas2D *canvas);
    std::vector<float> m_rowKernel;
    std::vector<float> m_colKernel;
    std::vector<BGRA> gradientMagnitude(std::vector<float> x, std::vector<float> y);
    inline unsigned char REAL2byte(float f);
};

#endif // FILTEREDGE_H
