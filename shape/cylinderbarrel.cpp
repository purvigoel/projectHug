#include "cylinderbarrel.h"

CylinderBarrel::CylinderBarrel()
{

}

CylinderBarrel::~CylinderBarrel()
{

}

std::vector<float> CylinderBarrel::buildShape(int numRows, int numCols){
    float stripHeight = 1.0f / numRows;
    std::vector<float> points;
    float i = -0.5f + stripHeight;
    for(int c = 0; c < numRows; c++){
        std::vector<float> rowpoints = defineHorizontalSlice(numCols, 0.5f, 0.5f, stripHeight, i, 0.0f, 0.0f);
        points.insert(points.end(), rowpoints.begin(), rowpoints.end());
        i += stripHeight;
    }
    return points;
}
