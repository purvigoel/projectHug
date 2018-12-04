#ifndef FILTER_H
#define FILTER_H
#include <vector>

#include <unistd.h>
#include <string>
#include <ui/Canvas2D.h>
#include "BGRA.h"

class Canvas2D;

class Filter
{
public:
    Filter(int xLo, int yLo, int xHi, int yHi);
    ~Filter();
    virtual std::vector<BGRA> applyFilter(Canvas2D *canvas) = 0;
    std::vector<float> convolve(std::vector<float> kernel, int kernelX, int kernelY, std::vector<float> in, int w, int h, bool transpose, bool threshold);
    inline unsigned char REAL2byte(float f);
    int m_xLo;
    int m_yLo;
    int m_xHi;
    int m_yHi;
};

#endif // FILTER_H
