#ifndef QuadraticBrush_Header
#define QuadraticBrush_Header

#include "Brush.h"


/**
 * @class QuadraticBrush
 *
 * Quadratic brush, whose mask has a falloff of 1/r^2.
 */
class QuadraticBrush
    : public Brush
{

public:
    QuadraticBrush(BGRA color, int radius);
    virtual ~QuadraticBrush();

    virtual void brushDown(int x, int y, Canvas2D *canvas) {}
    virtual void brushUp(int x, int y, Canvas2D *canvas) {}
    virtual void startTimer(Canvas2D *canvas){}
    virtual void createTimer(Canvas2D *canvas, int width, int height){}
    virtual void update(Canvas2D * canvas){}
    int getBrushType();
protected:
    void makeMask(); // Constructs the mask for this brush.
    float getMaskValue(int radius, float distanceFromRadius);

};

#endif
