#ifndef __BRUSH_H__    //header guards
#define __BRUSH_H__

#include <vector>

#include "BGRA.h"
#include <math.h>

class Canvas2D;

/**
 * @class Brush
 *
 * The superclass for all brushes.
 */
class Brush {
public:
    // Constructor for brush.  Pass it the initial color, flow and radius.
    Brush(BGRA color, int radius);

    // Destructor for brush.  Clean up all brush allocated resources.
    virtual ~Brush();

    BGRA getBGRA() const;
    int getAlpha() const;
    int getBlue() const;
    int getRed() const;
    int getGreen() const;
    int getRadius() const;

    void setAlpha(int alpha);
    void setBlue(int blue);
    void setGreen(int green);
    void setRed(int red);
    void setBGRA(const BGRA &bgra);
    void setRadius(int radius);

    virtual void brushDown(int x, int y, Canvas2D* canvas)=0;
    virtual void brushDragged(int x, int y, Canvas2D* canvas, bool alphaBlending);
    virtual void brushUp(int x, int y, Canvas2D* canvas) =0;
    virtual void startTimer(Canvas2D * canvas)=0;
    virtual void createTimer(Canvas2D * canvas, int width, int height)=0;
    virtual void update(Canvas2D * canvas)=0;
    void registerBrushDown(int x, int y, Canvas2D* canvas);
    void registerBrushUp(int x, int y, Canvas2D* canvas);
    virtual int getBrushType() = 0;
    virtual void handleTargetSquare(int x, int y, BGRA * pix, bool alphaBlending);
    float distance(int x, int y, int x2, int y2);
    BGRA blendColor(int index, BGRA oldColor, bool alpha);
    void blendLayers(BGRA * canvas, std::vector<BGRA> separateDrawingLayer);

protected:
    // Pure virtual function that will create the mask distribution.
    // To be be implemented in subclasses.
    virtual void makeMask() = 0;
    virtual float getMaskValue(int radius, float distanceFromRadius) = 0;
    // C++ coding convention is to put m_ in front of member variables
    BGRA m_color;
    std::vector<float> m_mask;
    std::vector<BGRA> m_separateDrawingLayer;
    std::vector<BGRA> m_oldCanvas;
    int m_radius;

};


#endif // __BRUSH_H__

