#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "primitiveinfo.h"
#include <vector>
#include <QWidget>
#include "SupportCanvas2D.h"
struct RayInfo{
    glm::mat4x4 modelMat;
    glm::vec3 cDiffuse;
    glm::vec3 cAmbient;
    glm::vec4 res;
    glm::vec3 normal;
    glm::vec4 d;
    glm::vec3 cSpecular;
    glm::vec3 cReflective;
    int id;
    glm::vec3 uv;
    CS123SceneFileMap texture;
    float blend;
    QImage * textureImage;
    float shiny;
    float intersect;
};


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();
    void paintImplicitScene(Scene &scene);

    float findObjectIntersection(PrimitiveInfo object, glm::vec4 p, glm::vec4 d);

    glm::vec3 findObjectNormal(PrimitiveInfo object, glm::vec3 point);

    glm::vec4 filmToWorld(glm::vec4 screen, glm::mat4x4 camToWorld);

    glm::vec4 screenToFilm(int screenX, int screenY, int canvasX, int canvasY);

    glm::vec4 generatePrimaryRay(int screenX, int screenY, int canvasX, int canvasY, glm::mat4x4 camToWorld);

    glm::vec4 constructDirectionVector(glm::vec4 worldPoint, glm::vec4 eyePoint);

    RayInfo getCollisionShape(glm::vec4 eye, glm::vec4 d, int primToSkip);

    glm::vec3 transformNormal(glm::mat4x4 object, glm::vec3 normal);

    std::vector<BGRA> recolorScene(int width, int height, glm::mat4x4 camToWorld, glm::vec4 eye);

    glm::vec4 rayToObjectSpace(PrimitiveInfo object, glm::vec4 ray);
    bool generateHardShadows(glm::vec4 surfacePoint, CS123SceneLightData *candidate, int primId);

    glm::vec3 recolorSceneRecursive(int width, int height, glm::mat4x4 camToWorld, glm::vec4 eye, glm::vec4 d, int recursionDepth, int primToSkip);
};


#endif // RAYSCENE_H
