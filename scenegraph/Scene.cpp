#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"

#include "glm/gtx/transform.hpp"
#include <iostream>

Scene::Scene()
{
    m_count = 1;
}

Scene::Scene(Scene &scene)
{

    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.
    if(scene.m_lightInfo.size() > 0){
        for(int i = 0; i < scene.m_lightInfo.size(); i ++){
            m_lightInfo.push_back(addLight(*scene.m_lightInfo[i]));
        }
    }

    if(scene.m_primitiveInfo.size() > 0){
        for(int i = 0; i < scene.m_primitiveInfo.size(); i ++){
            m_primitiveInfo.push_back(copyPrimitive(*scene.m_primitiveInfo[i]));
        }
    }
    if(scene.m_global){
        CS123SceneGlobalData globalStore = { scene.m_global->ka, scene.m_global->kd, scene.m_global->ks, scene.m_global->kt};
        setGlobal(globalStore);
    }

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D

    // but delete everything else.
    int numLights = m_lightInfo.size();
    for(int i = 0; i < numLights; i++){

        CS123SceneLightData* deleteLight = m_lightInfo[i];
        if(deleteLight){
            delete deleteLight;
        }
    }
    m_lightInfo.clear();
    for(int i = 0; i < m_primitiveInfo.size(); i ++){
        PrimitiveInfo *deletePrim = m_primitiveInfo[i];
        if(deletePrim){
            if(deletePrim->material.textureMap.isUsed && deletePrim->textureImage != nullptr){
                delete deletePrim->textureImage;
                deletePrim->textureImage = nullptr;
            }
            deletePrim->material.textureMap.clear();
            deletePrim->material.bumpMap.clear();
            if(deletePrim->imp){
                delete deletePrim->imp;
            }
            delete deletePrim;
        }
    }
    m_primitiveInfo.clear();
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()

    // Go through all necessary steps to set up scene.
    CS123SceneNode *rootNode = parser->getRootNode();
    CS123SceneGlobalData gData;
    parser->getGlobalData(gData);
    sceneToFill->setGlobal(gData);
    glm::mat4x4 ogMat = {1.0f,0.0f,0.0f,0.0f,
                         0.0f,1.0f,0.0f,0.0f,
                         0.0f,0.0f,1.0f,0.0f,
                         0.0f,0.0f,0.0f,1.0f};
    sceneToFill->lightMaker(parser, sceneToFill);
    sceneToFill->traverseTree(rootNode, ogMat, sceneToFill);

}

void Scene::traverseTree(CS123SceneNode *rootNode, glm::mat4x4 parentMat, Scene * toFill){
    std::vector<CS123SceneNode*> childrenList = rootNode->children;
    std::vector<CS123ScenePrimitive*> primitiveList = rootNode->primitives;
    std::vector<CS123SceneTransformation*> transList = rootNode->transformations;
    // current matrix transform for the current node, calculated using the node's transforms
    // on top of the parent node's transforms.
    glm::mat4x4 finalMat = compileMatrices(transList, parentMat);

    std::vector<PrimitiveInfo*> shapes = primitiveMaker(primitiveList, finalMat, toFill);
    for(int i = 0; i < shapes.size(); i ++){
        m_primitiveInfo.push_back(shapes[i]);
    }
    // We do this recursively for all children.
    for(int i = 0; i < childrenList.size(); i++){
        traverseTree(childrenList[i], finalMat, toFill);
    }

}

glm::mat4x4 Scene::compileMatrices(std::vector<CS123SceneTransformation*> transList, glm::mat4x4 tMat){
    glm::mat4x4 newMat;
    glm::mat4x4 construct = tMat;
    // These are the different types of transformations. We just need
    // to apply the correct matrix for the given transformation. We right-
    // multiply this new transformation matrix to the constructed one,
    // according to the handout: "multiply matrices in order in which they appear".
    for(int t = 0; t < transList.size(); t++){
        CS123SceneTransformation* currTrans = transList[t];
        TransformationType type = currTrans->type;
        if(type == TRANSFORMATION_TRANSLATE){
            glm::vec3 translate = currTrans->translate;
            newMat = {1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      translate.x, translate.y, translate.z, 1.0f};
            construct = construct * newMat;
        } else if (type == TRANSFORMATION_SCALE){
            glm::vec3 scale = currTrans->scale;
            newMat = {scale.x, 0.0f, 0.0f, 0.0f,
                      0.0f, scale.y, 0.0f, 0.0f,
                      0.0f, 0.0f, scale.z, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f};
            construct = construct * newMat;
        } else if (type == TRANSFORMATION_ROTATE){
            // We'll align the axes, perform the transformation
            // then move the axes back.
            glm::vec3 rotate = currTrans->rotate;
            float angle = currTrans->angle;
            glm::vec3 rotateNorm = glm::normalize(rotate);
            float d = sqrt(rotateNorm.z * rotateNorm.z + rotateNorm.y * rotateNorm.y);
            float cosT = rotateNorm.z / d;
            float sinT = rotateNorm.y / d;
            glm::mat4x4 rZ = {cos(angle), sin(angle), 0.0f, 0.0f,
                              -sin(angle), cos(angle), 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f};
            //ry
            glm::mat4x4 rY = {d, 0.0f, rotateNorm.x, 0.0f,
                              0.0f,1.0f,0.0f,0.0f,
                              -rotateNorm.x,0.0f,d,0.0f,
                              0.0f,0.0f,0.0f,1.0f};
            //rx
            glm::mat4x4 rX = {1.0f,0.0f,0.0f,0.0f,
                              0.0f, cosT, sinT,0.0f,
                              0.0f, -sinT, cosT,0.0f,
                              0.0f,0.0f,0.0f,1.0f};

            glm::mat4x4 rXinv = glm::inverse(rX);
            glm::mat4x4 rYinv = glm::inverse(rY);
            // if d is 0 then the x axes were aligned.
            if(d != 0){
                construct = construct * rXinv;
                construct = construct * rYinv;
                construct = construct * rZ;
                construct = construct * rY;
                construct = construct * rX;
            } else {
                construct = construct * rYinv;
                construct = construct * rZ;
                construct = construct * rY;
            }
        } else if (type == TRANSFORMATION_MATRIX){
            glm::mat4x4 mat = currTrans->matrix;
            construct = construct * mat;
        }
    }
    return construct;
}

std::vector<PrimitiveInfo*> Scene::primitiveMaker(std::vector<CS123ScenePrimitive*> pList, glm::mat4x4 pMat, Scene *toFill){
    std::vector<PrimitiveInfo*> primitives;
    for(int i = 0; i < pList.size(); i ++){
        PrimitiveInfo* p = toFill->addPrimitive(*pList[i], pMat);
        primitives.push_back(p);
    }
    return primitives;
}

// copy all light data for later user.
void Scene::lightMaker(CS123ISceneParser *parser, Scene *toFill){
    for(int i = 0; i < parser->getNumLights(); i++){
        CS123SceneLightData lightData;
        parser->getLightData(i, lightData);
        CS123SceneLightData * copyLightData = toFill->addLight(lightData);
        m_lightInfo.push_back(copyLightData);
    }
}

// Cleanly copy and store all primitive data for later use.
PrimitiveInfo* Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    PrimitiveType type = scenePrimitive.type;
    PrimitiveInfo* primitive = new PrimitiveInfo();
    primitive->id = m_count;
    m_count ++;
    primitive->type = type;
    primitive->meshfile = scenePrimitive.meshfile;
    primitive->mat = matrix;

    CS123SceneMaterial material;
    material.cDiffuse = scenePrimitive.material.cDiffuse * m_global->kd;
    material.cAmbient = scenePrimitive.material.cAmbient * m_global->ka;
    material.cReflective = scenePrimitive.material.cReflective * m_global->ks;
    material.cSpecular = scenePrimitive.material.cSpecular * m_global->ks;
    material.cTransparent = scenePrimitive.material.cTransparent * m_global->kt;
    material.cEmissive = scenePrimitive.material.cEmissive;


    CS123SceneFileMap textureMap;
    textureMap.isUsed = scenePrimitive.material.textureMap.isUsed;
    textureMap.filename = scenePrimitive.material.textureMap.filename;
    textureMap.repeatU = scenePrimitive.material.textureMap.repeatU;
    textureMap.repeatV = scenePrimitive.material.textureMap.repeatV;

    if(textureMap.isUsed){
        QImage *textureImage = new QImage();
        bool success = textureImage->load(textureMap.filename.c_str());
        if(success){
            primitive->textureImage = textureImage;
        } else{
            std::cout << "Couldn't load the image! Today is a sad day!!" << std::endl;
            primitive->textureImage = nullptr;
            textureMap.isUsed = false;
            if(textureImage){
                delete textureImage;
            }
        }
    } else {
        primitive->textureImage = nullptr;
    }

    material.textureMap = textureMap;
    material.blend = scenePrimitive.material.blend;

    CS123SceneFileMap bumpMap;
    bumpMap.isUsed = scenePrimitive.material.bumpMap.isUsed;
    bumpMap.filename = scenePrimitive.material.bumpMap.filename;
    bumpMap.repeatU = scenePrimitive.material.bumpMap.repeatU;
    bumpMap.repeatV = scenePrimitive.material.bumpMap.repeatV;
    material.bumpMap = bumpMap;

    material.shininess = scenePrimitive.material.shininess;

    material.ior = scenePrimitive.material.ior;
    primitive->material = material;
    return primitive;
}

PrimitiveInfo* Scene::copyPrimitive(PrimitiveInfo &scenePrimitive) {
    PrimitiveType type = scenePrimitive.type;
    // Alas!! a "new" keyword. We iterate through the list in the destructor to clean this up.
    PrimitiveInfo* primitive = new PrimitiveInfo();
    primitive->id = scenePrimitive.id;
    primitive->type = type;
    primitive->meshfile = scenePrimitive.meshfile;
    primitive->mat = scenePrimitive.mat;

    // Adjust material colors based on global data.
    CS123SceneMaterial material;
    material.cDiffuse = scenePrimitive.material.cDiffuse * m_global->kd;
    material.cAmbient = scenePrimitive.material.cAmbient * m_global->ka;
    material.cReflective = scenePrimitive.material.cReflective;
    material.cSpecular = scenePrimitive.material.cSpecular * m_global->ks;
    material.cTransparent = scenePrimitive.material.cTransparent * m_global->kt;
    material.cEmissive = scenePrimitive.material.cEmissive;

    CS123SceneFileMap textureMap;
    textureMap.isUsed = scenePrimitive.material.textureMap.isUsed;
    textureMap.filename = scenePrimitive.material.textureMap.filename;
    textureMap.repeatU = scenePrimitive.material.textureMap.repeatU;
    textureMap.repeatV = scenePrimitive.material.textureMap.repeatV;
    if(textureMap.isUsed){
        QImage *textureImage = new QImage();
        bool isLoaded = textureImage->load(textureMap.filename.c_str());
        if(isLoaded){
            primitive->textureImage = textureImage;
        } else {
            if(textureImage){
                delete textureImage;
            }
        }

    } else {
        primitive->textureImage = nullptr;
    }

    material.textureMap = textureMap;
    material.blend = scenePrimitive.material.blend;

    CS123SceneFileMap bumpMap;
    bumpMap.isUsed = scenePrimitive.material.bumpMap.isUsed;
    bumpMap.filename = scenePrimitive.material.bumpMap.filename;
    bumpMap.repeatU = scenePrimitive.material.bumpMap.repeatU;
    bumpMap.repeatV = scenePrimitive.material.bumpMap.repeatV;
    material.bumpMap = bumpMap;

    material.shininess = scenePrimitive.material.shininess;

    material.ior = scenePrimitive.material.ior;
    primitive->material = material;
    return primitive;
}

CS123SceneLightData* Scene::addLight(const CS123SceneLightData &sceneLight) {
    CS123SceneLightData *light = new CS123SceneLightData();
    light->id = sceneLight.id;
    light->type = sceneLight.type;
    light->color = sceneLight.color;
    light->function = sceneLight.function;
    light->pos = sceneLight.pos;
    light->dir = sceneLight.dir;
    light->radius = sceneLight.radius;
    light->penumbra = sceneLight.penumbra;
    light->angle = sceneLight.angle;
    light->width = sceneLight.width;
    light->height= sceneLight.height;
    return light;
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    CS123SceneGlobalData globalStore = { global.ka, global.kd, global.ks, global.kt};
    m_global = std::make_unique<CS123SceneGlobalData>(globalStore);
}

