#include "cylinder.h"
#include "cylinderbarrel.h"
#include "Settings.h"
Cylinder::Cylinder()
{
    m_barrel = std::make_unique<CylinderBarrel>();
    m_capTop = std::make_unique<Cap>();
    m_capBottom = std::make_unique<Cap>();
    m_shapeType = SHAPE_CYLINDER;
}
Cylinder::~Cylinder()
{

}
void Cylinder::initializeShape(float x, float y){
    if(y < 3) {
        y = 3;
    }

    // A cylinder is composed of a barrel and two caps. The caps
    // must be drawn in opposite directions.
    m_shapeParameterOne = x;
    m_shapeParameterTwo = y;
    m_barrel->initializeShape(x,y);
    m_barrel->draw();
    m_capTop->buildShape(y, 0.5f, 0.5f, true);
    m_capTop->draw();
    m_capBottom->buildShape(y, -0.5f, 0.5f, false);
    m_capBottom->draw();
}

void Cylinder::draw(){
    m_barrel->draw();
    m_capTop->draw();
    m_capBottom->draw();
}
