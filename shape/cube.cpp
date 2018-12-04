#include "cube.h"
#include <iostream>
#include "openglshape.h"
#include "Settings.h"
#include "cubebarrel.h"
#include "cubecap.h"
class Canvas2D;

Cube::Cube()
    : GLWidget()
{
    m_shapeType = SHAPE_CUBE;
    m_barrel = std::make_unique<CubeBarrel>();
    m_topCap = std::make_unique<CubeCap>();
    m_bottomCap = std::make_unique<CubeCap>();
}

Cube::~Cube()
{
}
void Cube::initializeShape(float x, float y) {
    m_shapeParameterOne = x;
    m_shapeParameterTwo = x;
    //a cube is composed of a barrel and two caps.
    m_barrel->initializeShape(x,x);
    m_barrel->draw();
    m_topCap->buildShape(x, 0.5, x, true);
    m_topCap->draw();
    m_bottomCap->buildShape(x, -0.5, x, false);
    m_bottomCap->draw();

}

void Cube::draw(){
    m_barrel->draw();
    m_topCap->draw();
    m_bottomCap->draw();
}
