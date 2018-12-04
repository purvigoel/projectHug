#ifndef IMPLICITSPHERE_H
#define IMPLICITSPHERE_H

#include "implicitshape.h"
class ImplicitSphere: public ImplicitShape
{
public:
    ImplicitSphere();
    virtual ~ImplicitSphere();
    virtual glm::vec3 getObjectNormal(glm::vec3 point);
    virtual float getIntersection(glm::vec4 p, glm::vec4 d);
    virtual glm::vec3 getTextureTarget(glm::vec3 point);
};

#endif // IMPLICITSPHERE_H
