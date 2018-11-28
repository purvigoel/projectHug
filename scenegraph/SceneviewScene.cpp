#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
#include "shape/cube.h"
#include "shape/sphere.h"
#include "shape/torus.h"
#include "shape/cylinder.h"
#include "shape/cone.h"
#include "shape/glwidget.h"
#include "CS123SceneData.h"


#include "glm/gtx/string_cast.hpp"
using namespace CS123::GL;


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();

    m_tesselated = false;
    std::cout << "scene load" << std::endl;
}

SceneviewScene::~SceneviewScene()
{
    std::cout << "deleting sceneview" << std::endl;
    int numShapes = m_shapeList.size();
    for(int i = 0; i < numShapes; i++){
        GLWidget *deleteShape = m_shapeList[i];
        if(deleteShape){
            delete deleteShape;
        }
    }
    m_shapeList.clear();
    m_tesselated = false;

}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::render(SupportCanvas3D *context) {
    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setSceneUniforms(context);
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

}

void SceneviewScene::setSceneUniforms(SupportCanvas3D *context) {
    Camera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());

}

void SceneviewScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void SceneviewScene::setLights()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //

    for(int i = 0; i < m_lightInfo.size(); i++){
        m_phongShader->setLight(*m_lightInfo[i]);
    }
}

void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // TODO: [SCENEVIEW] Fill this in...
    // You shouldn't need to write *any* OpenGL in this class!
    //
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //

    std::cout << "rendering geometry" << " " << m_primitiveInfo.size() << std::endl;


    if(m_tesselated == false){
        m_tesselated = true;
        for(int i = 0; i < m_primitiveInfo.size(); i++){
            PrimitiveInfo *p = m_primitiveInfo[i];
            m_phongShader->applyMaterial(p->material);
            m_phongShader->setUniform("m", p->mat);

            std::cout<<glm::to_string(p->mat)<<std::endl;
            GLWidget * c;
            switch(p->type){

            case PrimitiveType::PRIMITIVE_CUBE:
                c = new Cube();
                c->initializeShape(10,10);
                c->draw();
                break;
            case PrimitiveType::PRIMITIVE_CONE:
                c = new Cone();
                c->initializeShape(10,10);
                c->draw();
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                c = new Cylinder();
                c->initializeShape(10,10);
                c->draw();
                break;
            case PrimitiveType::PRIMITIVE_TORUS:
                c = new Torus();
                c->initializeShape(10,10);
                c->draw();
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                c = new Sphere();
                c->initializeShape(50,50);
                c->draw();
                break;
            case PrimitiveType::PRIMITIVE_MESH:
                c = new Cube();
                c->initializeShape(10,10);
                c->draw();
                break;
            default:
                    break;
            }
            m_phongShader->setUniform("m", glm::mat4(1.0f));
            m_shapeList.push_back(c);
        }
    } else {
        for(int i = 0; i < m_primitiveInfo.size(); i++){
            PrimitiveInfo *p = m_primitiveInfo[i];
            m_phongShader->applyMaterial(p->material);
            m_phongShader->setUniform("m", p->mat);
            m_shapeList[i]->draw();
        }
    }

}

void SceneviewScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.
}

