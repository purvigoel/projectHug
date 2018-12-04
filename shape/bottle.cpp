#include "bottle.h"
#include "bottlebarrel.h"

Bottle::Bottle()
{
    m_barrel = std::make_unique<BottleBarrel>();
    m_shapeType = SHAPE_SPECIAL_1;
}

Bottle::~Bottle()
{

}
void Bottle::initializeShape(float x, float y){
    if(x < 5){
        x = 5;
    }
    if(y < 5){
        y = 5;
    }
    m_shapeParameterOne = x;
    m_shapeParameterTwo = y;
    m_barrel->initializeShape(x,y);
    m_barrel->draw();

}

void Bottle::draw(){
    m_barrel->draw();

}
