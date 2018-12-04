#include "glwidget.h"
#include <math.h>
#include <iostream>
#include "ResourceLoader.h"
#include "openglshape.h"

#include "gl/shaders/ShaderAttribLocations.h"


using namespace CS123::GL;

#define PI 3.14159265

GLWidget::GLWidget():
    m_shape(nullptr)
{
    m_shape = std::make_unique<OpenGLShape>();\
}

GLWidget::~GLWidget()
{
}

int GLWidget::getShapeParameterOne(){
    return m_shapeParameterOne;
}

int GLWidget::getShapeParameterTwo(){
    return m_shapeParameterTwo;
}

int GLWidget::getShapeType(){
    return m_shapeType;
}

void GLWidget::draw(){
    if(m_shape){
        m_shape->draw();
    }
}
