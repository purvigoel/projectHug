#include "torus.h"
#include "torusbarrel.h"
Torus::Torus()
{
    m_barrel = std::make_unique<TorusBarrel>();
    m_shapeType = SHAPE_TORUS;
}


Torus::~Torus()
{

}
void Torus::initializeShape(float x, float y){
    if(x<3){
        x = 3;
    }
    if(y<3){
        y = 3;
    }
    // a torus is made of a barrel.
    m_shapeParameterOne = x;
    m_shapeParameterTwo = y;
    m_barrel->initializeShape(x,y);
    m_barrel->draw();
}

void Torus::draw(){
    m_barrel->draw();
}
