#ifndef TORUSBARREL_H
#define TORUSBARREL_H

#include "barrel.h"
class TorusBarrel : public Barrel
{
public:
    TorusBarrel();
    ~TorusBarrel();
    virtual std::vector<float> buildShape(int numRows, int numCols);

};

#endif // TORUSBARREL_H
