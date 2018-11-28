#include "cubecap.h"

CubeCap::CubeCap()
{

}

CubeCap::~CubeCap()
{

}

// Cube caps are square, rather than circular. We define these in a way familiar to
// LAYOUT_TRIANGLe
std::vector<float> CubeCap::buildShape(int numEdges, float y, float radius, bool direction){
    std::vector<float> cap;

    for(float i = 0; i < numEdges ; i ++){
        for(float j = 0; j < numEdges; j ++){
            cap.push_back(-0.5 + i/numEdges);
            cap.push_back(y);
            cap.push_back(-0.5 + j / numEdges);

            if(direction){

                cap.push_back(0);
                cap.push_back(1);
                cap.push_back(0);

                cap.push_back(-0.5 + i/numEdges);
                cap.push_back(y);
                cap.push_back(-0.5 + (j+1)/numEdges);

                cap.push_back(0);
                cap.push_back(1);
                cap.push_back(0);

                cap.push_back(-0.5 + (i+1)/numEdges);
                cap.push_back(y);
                cap.push_back(-0.5 + (j+1)/numEdges);

                cap.push_back(0);
                cap.push_back(1);
                cap.push_back(0);
            } else {
                cap.push_back(0);
                cap.push_back(-1);
                cap.push_back(0);

                cap.push_back(-0.5 + (i+1)/numEdges);
                cap.push_back(y);
                cap.push_back(-0.5 + (j+1)/numEdges);

                cap.push_back(0);
                cap.push_back(-1);
                cap.push_back(0);

                cap.push_back(-0.5 + i/numEdges);
                cap.push_back(y);
                cap.push_back(-0.5 + (j+1)/numEdges);

                cap.push_back(0);
                cap.push_back(-1);
                cap.push_back(0);
            }

            cap.push_back(-0.5 + i/numEdges);
            cap.push_back(y);
            cap.push_back(-0.5 + j / numEdges);

            if(direction){

                cap.push_back(0);
                cap.push_back(1);
                cap.push_back(0);

                cap.push_back(-0.5 + (i+1)/numEdges);
                cap.push_back(y);
                cap.push_back(-0.5 + (j+1)/numEdges);

                cap.push_back(0);
                cap.push_back(1);
                cap.push_back(0);

                cap.push_back(-0.5 + (i+1)/numEdges);
                cap.push_back(y);
                cap.push_back(-0.5 + (j)/numEdges);

                cap.push_back(0);
                cap.push_back(1);
                cap.push_back(0);
            }else{

                cap.push_back(0);
                cap.push_back(-1);
                cap.push_back(0);

                cap.push_back(-0.5 + (i+1)/numEdges);
                cap.push_back(y);
                cap.push_back(-0.5 + (j)/numEdges);

                cap.push_back(0);
                cap.push_back(-1);
                cap.push_back(0);

                cap.push_back(-0.5 + (i+1)/numEdges);
                cap.push_back(y);
                cap.push_back(-0.5 + (j+1)/numEdges);

                cap.push_back(0);
                cap.push_back(-1);
                cap.push_back(0);

            }
        }
    }

    m_shape->setVertexData(cap.data(), cap.size(),VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, cap.size()/3);
    m_shape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->buildVAO();
    return cap;
}
