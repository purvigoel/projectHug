#ifndef SPHERE_H
#define SPHERE_H

#include "glwidget.h"
#include "barrel.h"
#include "cap.h"
class Sphere : public GLWidget
{
public:
    Sphere();
    ~Sphere();
    virtual void initializeShape(float x, float y);
    virtual void draw();
protected:
    std::unique_ptr<Barrel> m_barrel;
    std::unique_ptr<Cap> m_cap;
};

#endif // CUBE_H
