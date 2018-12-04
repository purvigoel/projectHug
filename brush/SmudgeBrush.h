#ifndef SmudgeBrush_Header
#define SmudgeBrush_Header

#include "QuadraticBrush.h"

/**
 * @class SmudgeBrush
 *
 * Smudge brush, which smudges color values across the canvas as the mouse is dragged.
 */
class SmudgeBrush : public QuadraticBrush {

public:
    SmudgeBrush(BGRA color, int radius);
    virtual ~SmudgeBrush();

    virtual void brushDown(int x, int y, Canvas2D *canvas);
    virtual void brushDragged(int x, int y, Canvas2D *canvas, bool alphaBlending);
    virtual void brushUp(int x, int y, Canvas2D *canvas);
    virtual void startTimer(Canvas2D *canvas){}
    virtual void createTimer(Canvas2D *canvas, int width, int height){}
    virtual void update(Canvas2D * canvas){}
    int getBrushType();
protected:
    // Constructs the mask for this brush.
    void makeMask();
    float getMaskValue(int rad, float distanceFromRadius);
    BGRA smudgeColor(int index, BGRA newColor, float red, float blue, float green, float alpha);
    //! Copy paint in the bounding box of the brush from canvas to m_paint
    void pickUpPaint(int x, int y, Canvas2D* canvas);
    std::vector<BGRA> m_tempBuffer;
};

#endif
