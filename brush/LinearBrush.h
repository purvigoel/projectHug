#ifndef LinearBrush_Header
#define LinearBrush_Header


#include "Brush.h"

/**
 * @class LinearBrush
 *
 * Linear brush, whose mask has linear falloff.
 */
class LinearBrush
    : public Brush
{

public:
    LinearBrush(BGRA color, int radius);
    virtual ~LinearBrush();

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
