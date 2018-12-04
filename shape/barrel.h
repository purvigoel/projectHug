#ifndef BARREL_H
#define BARREL_H
#include "glwidget.h"

class Barrel : public GLWidget
{
public:
    Barrel();
    virtual ~Barrel();
    virtual void initializeShape(float x, float y);
    std::vector<float> defineHorizontalSlice(int numStrips, float radiusTop, float radiusBottom, float stripHeight, float stripY, float stripX, float stripZ);
    std::vector<float> defineCircle(int numEdges, float y, float radius, float slope);
    virtual std::vector<float> buildShape(int numRows, int numCols);
};

#endif // BARREL_H
