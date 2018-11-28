#ifndef IMPLICITCUBE_H
#define IMPLICITCUBE_H

#include "implicitshape.h"
class ImplicitCube: public ImplicitShape
{
public:
    ImplicitCube();
    virtual ~ImplicitCube();
    virtual glm::vec3 getTextureTarget(glm::vec3 point);

    virtual glm::vec3 getObjectNormal(glm::vec3 point);
    virtual float getIntersection(glm::vec4 p, glm::vec4 d);
};

#endif // IMPLICITCUBE_H
