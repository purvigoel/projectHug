#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"
#include "primitiveinfo.h"


#include <GL/glew.h>

#include <memory>
class Camera;
class CS123ISceneParser;


/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);
    glm::mat4x4 compileMatrices(std::vector<CS123SceneTransformation*> transList, glm::mat4x4 tMat);

    void traverseTree(CS123SceneNode *rootNode, glm::mat4x4 parentMat, Scene *toFill);
    std::vector<PrimitiveInfo*> primitiveMaker(std::vector<CS123ScenePrimitive*> pList, glm::mat4x4 pMat, Scene *toFill);
    void lightMaker(CS123ISceneParser *parser, Scene *toFill);
    PrimitiveInfo* copyPrimitive(PrimitiveInfo &scenePrimitive);
protected:

    // Adds a primitive to the scene.
    virtual PrimitiveInfo* addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual CS123SceneLightData* addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

    std::vector<PrimitiveInfo*> m_primitiveInfo;
    std::unique_ptr<CS123SceneGlobalData> m_global;
    std::vector<CS123SceneLightData *> m_lightInfo;

    int m_count;
};

#endif // SCENE_H
