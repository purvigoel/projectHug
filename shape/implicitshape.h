#ifndef IMPLICITSHAPE_H
#define IMPLICITSHAPE_H


#include <memory>

#include "GL/glew.h"
#include <QGLWidget>
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "CS123SceneData.h"

class ImplicitShape
{
public:
    ImplicitShape();
    virtual ~ImplicitShape();
    virtual glm::vec3 getObjectNormal(glm::vec3 point) = 0;
    virtual float getIntersection(glm::vec4 p, glm::vec4 d) = 0;

    std::vector<float> solveForT(float A, float B, float C);

    virtual glm::vec3 getTextureTarget(glm::vec3 point) = 0;
};

#endif // IMPLICITSHAPE_H
