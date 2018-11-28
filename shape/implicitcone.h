#ifndef IMPLICITCONE_H
#define IMPLICITCONE_H
#include "implicitshape.h"

class ImplicitCone: public ImplicitShape
{
public:
    ImplicitCone();
    virtual ~ImplicitCone();
    virtual glm::vec3 getObjectNormal(glm::vec3 point);
    virtual float getIntersection(glm::vec4 p, glm::vec4 d);
    virtual glm::vec3 getTextureTarget(glm::vec3 point);

};

#endif // IMPLICITCONE_H
