#ifndef CONE_H
#define CONE_H
#include "glwidget.h"
#include "barrel.h"
#include "cap.h"
class Cone : public GLWidget
{
public:
    Cone();
    ~Cone();
    virtual void initializeShape(float x, float y);
    virtual void draw();
protected:
    std::unique_ptr<Barrel> m_barrel;
    std::unique_ptr<Cap> m_cap;
};

#endif // CONE_H
