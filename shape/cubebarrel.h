#ifndef CUBEBARREL_H
#define CUBEBARREL_H

#include "barrel.h"
class CubeBarrel : public Barrel
{
public:
    CubeBarrel();
    ~CubeBarrel();
    virtual std::vector<float> buildShape(int numRows, int numCols);
    virtual std::vector<float> defineHorizontalSlice(int numStrips, float radiusTop, float radiusBottom, float stripHeight, float stripY, float stripX, float stripZ);

};

#endif // CUBEBARREL_H
