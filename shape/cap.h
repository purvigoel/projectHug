#ifndef CAP_H
#define CAP_H

#include "glwidget.h"

class Cap : public GLWidget
{
public:
    Cap();
    virtual ~Cap();
    virtual void initializeShape(float x, float y) {}
    virtual std::vector<float> buildShape(int numEdges, float y, float radius, bool direction);
    virtual std::vector<float> buildShape(int numEdges, int y){}
};

#endif // CAP_H
