#include "spherecap.h"

SphereCap::SphereCap()
{

}

// Quite similar to a normal cap, just the center of the triangle fan needs to jut out
// a little.
std::vector<float> SphereCap::buildShape(int numEdges, float y, float radius, bool direction){
    float circleDegrees = M_PI*2;
    float anglePerTriangle = circleDegrees/numEdges;
    std::vector<float> cap;
    cap.push_back(0.0f);
    cap.push_back(0.0f);
    cap.push_back(-0.5f);
    int start = numEdges;
    int end = 0;
    int itr = -1;
    if(!direction){
        start = 0;
        end = numEdges;
        itr = 1;
    }
    float norm = pow(0.5 * 0.5,0.5);
    cap.push_back(0.0/norm);
    cap.push_back(0.0/norm);
    cap.push_back(-0.5/norm);
    for(int i = start; i != end + itr; i += itr){
        float x = radius * cos(i * anglePerTriangle);
        float z = radius * sin(i * anglePerTriangle);
        cap.push_back(x);
        cap.push_back(z);
        cap.push_back(y);
        norm = x * x + y * y + z * z;
        norm = pow(norm, 0.5);
        cap.push_back(x/norm);
        cap.push_back(z/norm);
        cap.push_back(y/norm);
    }
    float x = radius * cos(0 * anglePerTriangle);
    float z = radius * sin(0 * anglePerTriangle);
    cap.push_back(x);
    cap.push_back(z);
    cap.push_back(y);
    norm = pow(0.5*0.5,0.5);
    cap.push_back(x/norm);
    cap.push_back(z/norm);
    cap.push_back(y/norm);
    m_shape->setVertexData(cap.data(), cap.size(),VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_FAN, cap.size()/3);
    m_shape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->buildVAO();
    return cap;
}
