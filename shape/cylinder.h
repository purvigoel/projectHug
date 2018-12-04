#ifndef CYLINDER_H
#define CYLINDER_H

#include "glwidget.h"
#include "barrel.h"
#include "cap.h"
class Cylinder : public GLWidget
{
public:
    Cylinder();
    ~Cylinder();
    virtual void initializeShape(float x, float y);
    virtual void draw();
protected:
    std::unique_ptr<Barrel> m_barrel;
    std::unique_ptr<Cap> m_capTop;
    std::unique_ptr<Cap> m_capBottom;
};


#endif // CYLINDER_H
