#include "cap.h"

Cap::Cap()
{

}

Cap::~Cap(){

}

// buildShape will build a VAO with the points of the "cap" of the shape, or the Base.
// The base will have a radius, y value, and number of edges defined by the user. the
// direction refers to whether we are drawing clockwise or counterclockwise. We draw
// the base in an orientation familiar to TRIANGLE FAN
std::vector<float> Cap::buildShape(int numEdges, float y, float radius, bool direction){
    float circleDegrees = M_PI*2;
    float anglePerTriangle = circleDegrees/numEdges;
    std::vector<float> cap;
    cap.push_back(0.0f);
    cap.push_back(y);
    cap.push_back(0.0f);
    int start = numEdges;
    int end = 0;
    int itr = -1;
    if(!direction){
        start = 0;
        end = numEdges;
        itr = 1;
    }
    cap.push_back(0.0);
    cap.push_back(-itr);
    cap.push_back(0.0);
    for(int i = start; i != end + itr; i += itr){
        float x = radius * cos(i * anglePerTriangle);
        float z = radius * sin(i * anglePerTriangle);
        cap.push_back(x);
        cap.push_back(y);
        cap.push_back(z);

        cap.push_back(0.0);
        cap.push_back(-itr);
        cap.push_back(0.0);
    }
    float x = radius * cos(0 * anglePerTriangle);
    float z = radius * sin(0 * anglePerTriangle);
    cap.push_back(x);
    cap.push_back(y);
    cap.push_back(z);
    cap.push_back(0.0);
    cap.push_back(-itr);
    cap.push_back(0.0);
    m_shape->setVertexData(cap.data(), cap.size(),VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_FAN, cap.size()/3);
    m_shape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->buildVAO();
    return cap;
}
