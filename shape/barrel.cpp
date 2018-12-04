#include "barrel.h"

Barrel::Barrel()
{

}

Barrel::~Barrel()
{

}

// We draw the barrel horizontal slice by horizontal slice. Each slice is defined by the y value.
// This lets us use Triangle strip.
std::vector<float> Barrel::defineHorizontalSlice(int numStrips, float radiusTop, float radiusBottom, float stripHeight, float stripY, float stripX, float stripZ) {
    float slope = (radiusBottom - radiusTop)/stripHeight;
    std::vector<float> capTop = defineCircle(numStrips,stripY, radiusTop, slope);
    std::vector<float> capBottom = defineCircle(numStrips,stripY - stripHeight, radiusBottom, slope);
    std::vector<float> points;
    for(int i = 3; i < capTop.size() ; i+=6){
        float x = capBottom[i];
        float y = capBottom[i + 1];
        float z = capBottom[i + 2];

        points.push_back(x);
        points.push_back(y);
        points.push_back(z);
        //normal
        points.push_back(capBottom[i+3]);
        points.push_back(capBottom[i+4]);
        points.push_back(capBottom[i+5]);

        x = capTop[i];
        y = capTop[i + 1];
        z = capTop[i + 2];

        points.push_back(x);
        points.push_back(y);
        points.push_back(z);
        //normal
        points.push_back(capTop[i+3]);
        points.push_back(capTop[i+4]);
        points.push_back(capTop[i+5]);
    }
    return points;
}

//initialize and build the VAO
void Barrel::initializeShape(float x, float y){
    std::vector<float> points = buildShape(x,y);
    int numVertices = points.size()/6;
    m_shape->setVertexData(points.data(), points.size(),VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, numVertices);
    m_shape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->buildVAO();
}

// Find how many horizontal shapes we need to draw and create a list of points oriented
// in a way familiar to TRIANGLE_STRIP
std::vector<float> Barrel::buildShape(int numRows, int numCols){
    float stripHeight = 1.0f/numRows;
    float radius = 0.5f/numRows;
    float splices = numCols;
    std::vector<float> points;
    float currY = 0.5;
    for(float i = 0.0f; i < 0.5f; i += radius){
        std::vector<float> rowpoints = defineHorizontalSlice(splices, i, i + radius, stripHeight, currY, 0.0f, 0.0f);
        points.insert(points.end(), rowpoints.begin(), rowpoints.end());
        currY -= stripHeight;
    }
    return points;
}

// The hoizontal strips will be defined by the borders of circles oriented at different y values.
// Here we find out what points are on these circles.
std::vector<float> Barrel::defineCircle(int numEdges, float y, float radius, float slope){
    float circleDegrees = 3.14*2;
    float anglePerTriangle = circleDegrees/numEdges;
    std::vector<float> cap;
    cap.push_back(0.0f);
    cap.push_back(y);
    cap.push_back(0.0f);

    for(int i = 0; i < numEdges; i ++){
        float x = radius * cos(i * anglePerTriangle);
        float z = radius * sin(i * anglePerTriangle);
        cap.push_back(x);
        cap.push_back(y);
        cap.push_back(z);

        cap.push_back( cos(i * anglePerTriangle));
        cap.push_back(slope);
        cap.push_back( sin(i * anglePerTriangle));
    }
    float x = radius * cos(0 * anglePerTriangle);
    float z = radius * sin(0 * anglePerTriangle);
    cap.push_back(x);
    cap.push_back(y);
    cap.push_back(z);
    cap.push_back(1.0);
    cap.push_back(slope);
    cap.push_back(0.0);
    return cap;
}
