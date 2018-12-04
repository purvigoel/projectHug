#ifndef BOTTLE_H
#define BOTTLE_H

#include "glwidget.h"
#include "barrel.h"
#include "cap.h"
class Bottle : public GLWidget
{
public:
    Bottle();
    ~Bottle();
    virtual void initializeShape(float x, float y);
    virtual void draw();
protected:
    std::unique_ptr<Barrel> m_barrel;
};

#endif // BOTTLE_H
