#include "sphere.h"
#include "spherebarrel.h"
#include "torusbarrel.h"
#include "spherecap.h"
Sphere::Sphere()
{
    m_barrel = std::make_unique<SphereBarrel>();
    m_cap = std::make_unique<SphereCap>();
    m_shapeType = SHAPE_SPHERE;
}

Sphere::~Sphere()
{

}
void Sphere::initializeShape(float x, float y){
    if(x < 3){
        x = 3;
    }
    if(y < 2){
        y = 2;
    }
    m_shapeParameterOne = x;
    m_shapeParameterTwo = y;
    m_barrel->initializeShape(x,y);
    m_barrel->draw();
    // A sphere is composed of a barrel and a cap. We calculate the
    // correct y value and radius for the cap here.
    float thetaIncrement = M_PI/y;
    float phi = 0.0;
    float yCap = -0.5 * sin(M_PI/2.0 - thetaIncrement);
    float xCap = 0.5 * cos(M_PI/2.0 - thetaIncrement) * sin(M_PI/2);
    m_cap->buildShape(x,yCap, xCap, true);
    m_cap->draw();
}

void Sphere::draw(){
    m_barrel->draw();
    m_cap->draw();
}
