#ifndef QUAD_H
#define QUAD_H

#include "glwidget.h"
class Quad : public GLWidget
{
public:
    Quad();
    ~Quad();
    virtual void initializeShape(float x, float y);
    virtual void draw();
};

#endif // QUAD_H
