#include "cone.h"
#include "barrel.h"
#include "Settings.h"
Cone::Cone()
{
    m_barrel = std::make_unique<Barrel>();
    m_cap = std::make_unique<Cap>();
    m_shapeType = SHAPE_CONE;
}
Cone::~Cone(){

}
void Cone::initializeShape(float x, float y){
    if(y < 3){
        y = 3;
    }
    //a cone is composed of a barrel and a cap.
    m_shapeParameterOne = x;
    m_shapeParameterTwo = y;
    m_barrel->initializeShape(x,y);
    m_barrel->draw();
    m_cap->buildShape(y, -0.5f, 0.5f, false);
    m_cap->draw();
}

void Cone::draw(){
    m_barrel->draw();
    m_cap->draw();
}
