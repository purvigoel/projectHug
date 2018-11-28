#ifndef FILTERSCALE_H
#define FILTERSCALE_H

#include "filter.h"
class FilterScale : public Filter
{
public:
    FilterScale(int xLo, int yLo, int xHi, int yHi);
    ~FilterScale();
    virtual std::vector<BGRA> applyFilter(Canvas2D *canvas);
    std::vector<BGRA> scaleX(float scaleBy, float currentXLength, float currentYLength, Canvas2D * canvas);
    float triFilter(float point, float a);
    BGRA hPrime(int k, float a, BGRA *data, int startX, int width);
    std::vector<BGRA> scaleY(float scaleBy, float currentXLength, float currentYLength, Canvas2D * canvas);
    void cropMarqueeArea(Canvas2D * canvas);
    void paintCanvas(Canvas2D * canvas, std::vector<BGRA> res);
    BGRA hPrimeY(int k, float a, BGRA *data, int widthTillNow, int width, int height);
};

#endif // FILTERSCALE_H
