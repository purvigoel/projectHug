#ifndef IMPLICITCYLINDER_H
#define IMPLICITCYLINDER_H

#include "implicitshape.h"
class ImplicitCylinder: public ImplicitShape
{
public:
    ImplicitCylinder();
    virtual ~ImplicitCylinder();
    virtual glm::vec3 getTextureTarget(glm::vec3 point);

    virtual glm::vec3 getObjectNormal(glm::vec3 point);
    virtual float getIntersection(glm::vec4 p, glm::vec4 d);
};

#endif // IMPLICITCYLINDER_H
