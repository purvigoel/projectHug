#include "cubebarrel.h"

CubeBarrel::CubeBarrel()
{

}

CubeBarrel::~CubeBarrel()
{

}

std::vector<float> CubeBarrel::buildShape(int numRows, int numCols){
    float stripHeight = 1.0f / numRows;
    std::vector<float> points;
    float i = -0.5 + stripHeight;
    for(int c = 0; c < numRows; c++){
        std::vector<float> rowpoints = defineHorizontalSlice(numCols, 1.0, 0.0, stripHeight, i, -0.5f, 0.5f);
        points.insert(points.end(), rowpoints.begin(), rowpoints.end());
        i += stripHeight;
    }
    return points;
}

// Cube Barrels aren't defined by circles, so we create them a little differently here. Here, we
// define each horizontal slice around a square shaped base.
std::vector<float> CubeBarrel::defineHorizontalSlice(int numStrips, float baseTop, float baseBottom, float stripHeight, float stripY, float stripX, float stripZ) {
    //Left face
    float stripWidth = baseTop/numStrips;
    std::vector<float> points;
    //front face
    for(int i = 0; i < numStrips + 1; i ++){
        points.push_back(float(stripX + stripWidth * i));
        points.push_back(float(stripY));
        points.push_back(float(stripZ));

        points.push_back(float(0));
        points.push_back(float(0));
        points.push_back(float(1));

        points.push_back(float(stripX + stripWidth * i));
        points.push_back(float(stripY - stripHeight));
        points.push_back(float(stripZ));

        points.push_back(float(0));
        points.push_back(float(0));
        points.push_back(float(1));
    }
    for(int i = 1; i < numStrips + 1; i ++){
        points.push_back(stripX + baseTop);
        points.push_back(float(stripY));
        points.push_back(float(stripZ - stripWidth * (i)));
        points.push_back(float(1));
        points.push_back(float(0));
        points.push_back(float(0));

        points.push_back(float(stripX + baseTop));
        points.push_back(float(stripY - stripHeight));
        points.push_back(float(stripZ - stripWidth * (i)));
        points.push_back(float(1));
        points.push_back(float(0));
        points.push_back(float(0));
    }

    for(int i = 1; i < numStrips + 1; i ++){
        points.push_back(float(stripX + baseTop - stripWidth * i));
        points.push_back(float(stripY));
        points.push_back(stripX);
        points.push_back(float(0));
        points.push_back(float(0));
        points.push_back(float(-1));

        points.push_back(float(stripX + baseTop - stripWidth * i));
        points.push_back(float(stripY - stripHeight));
        points.push_back(stripX);
        points.push_back(float(0));
        points.push_back(float(0));
        points.push_back(float(-1));
    }

    for(int i = 1; i < numStrips + 1; i ++){
        points.push_back(stripX);
        points.push_back(float(stripY));
        points.push_back(float(stripZ - baseTop +  stripWidth * (i)));
        points.push_back(float(-1));
        points.push_back(float(0));
        points.push_back(float(0));
        points.push_back(float(stripX));
        points.push_back(float(stripY - stripHeight));
        points.push_back(float(stripZ - baseTop + stripWidth * (i)));
        points.push_back(float(-1));
        points.push_back(float(0));
        points.push_back(float(0));
    }

    return points;
    // TODO [Task 7]
}

