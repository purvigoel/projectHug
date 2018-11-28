#ifndef CYLINDERBARREL_H
#define CYLINDERBARREL_H

#include "barrel.h"

class CylinderBarrel : public Barrel
{
public:
    CylinderBarrel();
    ~CylinderBarrel();
    virtual std::vector<float> buildShape(int numRows, int numCols);
};

#endif // CYLINDERBARREL_H
