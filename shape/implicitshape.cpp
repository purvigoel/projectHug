#include "implicitshape.h"
#include "math.h"
#include <iostream>
ImplicitShape::ImplicitShape()
{

}

ImplicitShape::~ImplicitShape()
{

}

std::vector<float> ImplicitShape::solveForT(float A, float B, float C){
    float sqRt = (B * B - 4 * A * C);

    std::vector<float> result;
    if(sqRt < 0.0){
        return result;
    }
    if(A == 0.0){
        assert(1 == 0);
        result.push_back(-C/B);
        return result;
    }

    float t1 = (-1.0 * B + sqrt(sqRt))/(2.0 *A);
    float t2 = (-1.0 * B - sqrt(sqRt))/(2.0 * A);

    if(t1 > 0 ){
        result.push_back(t1);
    }
    if(t2 > 0){
        result.push_back(t2);
    }
    return result;
}
