#ifndef CUBE_H
#define CUBE_H

#include "glwidget.h"
#include "barrel.h"
#include "cap.h"
class Cube : public GLWidget
{
public:
    Cube();
    ~Cube();
    virtual void initializeShape(float x, float y);
    virtual void draw();
protected:
    std::unique_ptr<Barrel> m_barrel;
    std::unique_ptr<Cap> m_topCap;
    std::unique_ptr<Cap> m_bottomCap;
};

#endif // CUBE_H
