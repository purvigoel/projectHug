#ifndef SPHEREBARREL_H
#define SPHEREBARREL_H

#include "barrel.h"
class SphereBarrel : public Barrel
{
public:
    SphereBarrel();
    ~SphereBarrel();
    virtual std::vector<float> buildShape(int numRows, int numCols);

};

#endif // SPHEREBARREL_H
