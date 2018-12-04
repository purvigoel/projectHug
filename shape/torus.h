#ifndef TORUS_H
#define TORUS_H


#include "glwidget.h"
#include "barrel.h"
class Torus : public GLWidget
{
public:
    Torus();
    ~Torus();
    virtual void initializeShape(float x, float y);
    virtual void draw();
protected:
    std::unique_ptr<Barrel> m_barrel;
};

#endif // TORUS_H
