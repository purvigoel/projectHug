#ifndef SpecialBrush_Header
#define SpecialBrush_Header

#include "Brush.h"
#include <QTimer>
class Canvas2D;
/**
 * @class SpecialBrush
 *
 * Constant brush with equal mask distribution in a circle.
 */
class SpecialBrush
    : public Brush
{

public:
    SpecialBrush(BGRA color, int radius);
    virtual ~SpecialBrush();

    virtual void brushDown(int x, int y, Canvas2D *canvas);
    virtual void brushUp(int x, int y, Canvas2D *canvas);
   virtual void brushDragged(int x, int y, Canvas2D *canvas, bool alphaBlending);
    int getBrushType();
    std::vector<float> noise(int canvasWidth, int canvasHeight);
    void createTimer(Canvas2D *canvas , int width, int height);
    void update(Canvas2D * canvas);
    void startTimer(Canvas2D * canvas);
  //  void animate();
protected:
    void makeMask(); // Constructs the mask for this brush.
    float getMaskValue(int radius, float distanceFromRadius);
    std::vector<float> m_noise;
std::vector<float> m_xs;
std::vector<float> m_ys;
bool m_shouldAnimate;
QTimer *m_timer;

};


#endif
