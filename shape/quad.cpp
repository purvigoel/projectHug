#include "quad.h"
#include <iostream>
Quad::Quad()
{

}

Quad::~Quad()
{

}
void Quad::initializeShape(float x, float y){
    float stripWidth = 8;
    int stripX = -4;
    int stripY = 3.0;
    int numStrips = 1;
    int stripZ = 0;
    int stripHeight = 8;
    std::vector<float> points;
    //front face
//    for(int i = 0; i < numStrips + 1; i ++){
//        points.push_back(float(stripX + stripWidth * i));
//        points.push_back(float(stripY));
//        points.push_back(float(stripZ));

//        points.push_back(0.0f);
//        points.push_back(0.0f);

//        points.push_back(float(stripX + stripWidth * i));
//        points.push_back(float(stripY - stripHeight));
//        points.push_back(float(stripZ));

//        points.push_back(1.0f);
//        points.push_back(1.0f);

//    }

//    int numVertices = points.size()/3;
//    m_shape->setVertexData(points.data(), points.size(),VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, numVertices);
//    m_shape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
//    m_shape->setAttribute(ShaderAttrib::TEXCOORD0, 2, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
//    m_shape->buildVAO();
    std::vector<GLfloat> quadData = {-4.0f,3.0f, 0.0f, 0.0f,1.0f,
                                     -4.0f,-5.0f, 0.0f, 0.0f,0.0f,
                                     4.0f, 3.0f, 0.0f, 1.0f,1.0f,
                                     4.0f,-5.0f, 0.0f, 1.0f, 0.0f };
    m_shape = std::make_unique<OpenGLShape>();
    m_shape->setVertexData(&quadData[0], quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_shape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->setAttribute(ShaderAttrib::TEXCOORD0, 2, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->buildVAO();



}

void Quad::draw(){
    m_shape->draw();
}
