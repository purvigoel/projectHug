#ifndef ConstantBrush_Header
#define ConstantBrush_Header

#include "Brush.h"

/**
 * @class ConstantBrush
 *
 * Constant brush with equal mask distribution in a circle.
 */
class ConstantBrush
    : public Brush
{

public:
    ConstantBrush(BGRA color, int radius);
    virtual ~ConstantBrush();

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
