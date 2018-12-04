#ifndef BOTTLEBARREL_H
#define BOTTLEBARREL_H
#include "barrel.h"
class BottleBarrel : public Barrel
{
public:
    BottleBarrel();
    virtual ~BottleBarrel();
    virtual std::vector<float> buildShape(int numRows, int numCols);
};

#endif // BOTTLEBARREL_H
