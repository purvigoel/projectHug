#include "ShapesScene.h"
#include "Camera.h"
#include <SupportCanvas3D.h>
#include <QFileDialog>

#include <sstream>
#include <iostream>


using namespace CS123::GL;
#include "gl/shaders/CS123Shader.h"
#include "gl/shaders/Shader.h"

#include "ResourceLoader.h"

#include "shape/cube.h"
#include "shape/sphere.h"
#include "shape/cylinder.h"

#include "shape/cone.h"
#include "shape/glwidget.h"
#include "shape/torus.h"

#include "shape/quad.h"
#include "gl/datatype/FBO.h"
#include "gl/textures/Texture2D.h"
#include "glm/ext.hpp"
ShapesScene::ShapesScene(int width, int height) :
    m_width(width),
    m_height(height)
{
    initializeSceneMaterial();
    initializeSceneLight();
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();
    std::cout << "shapes" << std::endl;

    //TODO: [SHAPES] Allocate any additional memory you need...
    m_widget = new Cone();

    m_worm = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY, m_width, m_height);
    m_fill = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY, m_width, m_height);
    m_fill2 = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY, m_width, m_height);
    m_horizontalBlur = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY, m_width, m_height);
    std::cout << m_width << " " << m_height << std::endl;
    setUpImage();
    //setUpDensityImage();
    m_renderTimes = 0;

}

ShapesScene::~ShapesScene()
{
    if(m_widget){
        delete m_widget;
    }
    // Pro-tip: If you use smart pointers properly, this destructor should be empty
}

void ShapesScene::initializeSceneMaterial() {
    // Use a shiny orange material
    m_material.clear();
    m_material.cAmbient.r = 0.2f;
    m_material.cAmbient.g = 0.1f;
    m_material.cDiffuse.r = 1.0f;
    m_material.cDiffuse.g = 0.5f;
    m_material.cSpecular.r = m_material.cSpecular.g = m_material.cSpecular.b = 1;
    m_material.shininess = 64;
}

void ShapesScene::initializeSceneLight() {
    // Use a white directional light from the upper left corner
    memset(&m_light, 0, sizeof(m_light));
    m_light.type = LightType::LIGHT_DIRECTIONAL;
    m_light.dir = m_lightDirection;
    m_light.color.r = m_light.color.g = m_light.color.b = 1;
    m_light.id = 0;
}

void ShapesScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);

    m_phongShader->setUniform("timer", 5);

    int sphereNum = 6;
    QImage image(sphereNum, 1, QImage::Format_RGBA8888);
    for(int i = 0; i < sphereNum; i++ ){ }

    m_image = image;

    std::string vertexSourceH = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenquad.vert");
    std::string fragmentSourceH = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_horizontalBlurShader = std::make_unique<Shader>(vertexSourceH, fragmentSourceH);

}


void ShapesScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenquad.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenquad.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void ShapesScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void ShapesScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenquad.vert");
   // std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void ShapesScene::render(SupportCanvas3D *context) {
    // Clear the screen in preparation for the next frame. (Use a gray background instead of a
    // black one for drawing wireframe or normals so they will show up against the background.)
    setClearColor();

    renderPhongPass(context);

//    if (settings.drawWireframe) {
//        renderWireframePass(context);
//    }

//    if (settings.drawNormals) {
//        renderNormalsPass(context);
//    }

}

void ShapesScene::renderPhongPass(SupportCanvas3D *context) {
  //  m_worm->bind();

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    m_worm->bind();
   //glViewport(0,0,m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);


    m_phongShader->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    clearLights();
    setLights(context->getCamera()->getViewMatrix());
    setPhongSceneUniforms();
    setMatrixUniforms(m_phongShader.get(), context);
    renderGeometryAsFilledPolygons();
    m_phongShader->unbind();

    m_worm->unbind();

    m_wireframeShader->bind();

    //glViewport(0,0,m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    m_worm->getColorAttachment(0).bind();

    m_fill->bind();
    m_wireframeShader->bind();

    //glViewport(0,0,m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    setMatrixUniforms(m_wireframeShader.get(), context);
    m_wireframeShader.get()->setUniform("searchWidth", 30);
    renderGeometry();
    m_worm->getColorAttachment(0).unbind();

    m_fill->unbind();

    m_fill2->bind();
    m_wireframeShader->bind();

    //glViewport(0,0,m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    m_fill->getColorAttachment(0).bind();
    setMatrixUniforms(m_wireframeShader.get(), context);
    m_wireframeShader.get()->setUniform("searchWidth",  4);
    m_wireframeShader.get()->setUniform("finalFill", 1);
    renderGeometry();



    m_fill->getColorAttachment(0).unbind();
    m_fill2->unbind();


    m_wireframeShader->unbind();

    m_horizontalBlur->bind();

    m_horizontalBlurShader->bind();

    //glViewport(0,0,m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    m_fill2->getColorAttachment(0).bind();
    setMatrixUniforms(m_horizontalBlurShader.get(), context);
    m_horizontalBlurShader.get()->setUniform("speckle", 0);

    renderGeometry();

    m_fill2->getColorAttachment(0).unbind();

    m_horizontalBlur->unbind();

    m_horizontalBlurShader->unbind();
    m_normalsArrowShader->bind();

    glViewport(0,0,m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    m_horizontalBlur->getColorAttachment(0).bind();


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, newHandle);
    GLint secondTextureLocation = m_normalsArrowShader->getTextureLocation("texture2");
    glUniform1i(secondTextureLocation,1);


    m_normalsArrowShader->setUniform("timer", m_renderTimes);
    m_renderTimes +=1;
    setMatrixUniforms(m_normalsArrowShader.get(), context);
    renderGeometry();
    m_horizontalBlur->getColorAttachment(0).unbind();

    glActiveTexture(GL_TEXTURE0);
}

void ShapesScene::setUpImage(){
    // Qimage image(file);
    QImage *image = new QImage();
    bool isLoaded = image->load("/home/pgoel2/course/cs1230/projects/tiger.png");
    glActiveTexture(GL_TEXTURE0 + 1);
    glGenTextures(1, &newHandle);
    glBindTexture(GL_TEXTURE_2D, newHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glActiveTexture(GL_TEXTURE0);
}

void ShapesScene::setUpDensityImage(){
    QImage *image = new QImage();
    bool isLoaded = image->load("/course/cs123/data/image/chessboard.png");
    std::cout << "density is loaded" << isLoaded << std::endl;
    glActiveTexture(GL_TEXTURE0 + 2);
    glGenTextures(1, &densityHandle);
    glBindTexture(GL_TEXTURE_2D, densityHandle);
    std::cout << "densityhandle" << densityHandle << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glActiveTexture(GL_TEXTURE0);
}

void ShapesScene::setPhongSceneUniforms() {
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->applyMaterial(m_material);
}

void ShapesScene::updateShader(int i){
    std::cout << i << std::endl;

    m_phongShader->bind();
    m_phongShader->setUniform("timer", i);
    renderGeometryAsFilledPolygons();
    m_phongShader->unbind();


}

void ShapesScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
    shader->setUniform("m", glm::mat4(1.0f));
}

void ShapesScene::renderGeometryAsFilledPolygons() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    renderGeometry();
}

void ShapesScene::renderWireframePass(SupportCanvas3D *context) {
    m_wireframeShader->bind();
    setMatrixUniforms(m_wireframeShader.get(), context);
    renderGeometryAsWireframe();
    m_wireframeShader->unbind();
}

void ShapesScene::renderGeometryAsWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    renderGeometry();
}

void ShapesScene::renderNormalsPass (SupportCanvas3D *context) {
    // Render the lines.
    m_normalsShader->bind();
    setMatrixUniforms(m_normalsShader.get(), context);
    renderGeometryAsWireframe();
    m_normalsShader->unbind();

    // Render the arrows.
    m_normalsArrowShader->bind();
    setMatrixUniforms(m_normalsArrowShader.get(), context);
    renderGeometryAsFilledPolygons();
    m_normalsArrowShader->unbind();
}

void ShapesScene::renderGeometry() {
    // TODO: [SHAPES] Render the shape. Lab 1 seems like it'll come in handy...
    if(m_widget){
        m_widget->draw();
    }
}

void ShapesScene::clearLights() {
    for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
        std::ostringstream os;
        os << i;
        std::string indexString = "[" + os.str() + "]"; // e.g. [0], [1], etc.
        m_phongShader->setUniform("lightColors" + indexString, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void ShapesScene::setLights(const glm::mat4 viewMatrix) {
    // YOU DON'T NEED TO TOUCH THIS METHOD, unless you want to do fancy lighting...

    m_light.dir = glm::inverse(viewMatrix) * m_lightDirection;

    clearLights();
    m_phongShader->setLight(m_light);
}

void ShapesScene::settingsChanged() {
    // TODO: [SHAPES] Fill this in if applicable.
    changeShapeType();
    changeShapeParameter();
}

void ShapesScene::changeShapeType(){
    if(settings.shapeType != m_widget->getShapeType()){
        delete m_widget;
        if(settings.shapeType == SHAPE_CUBE){
            m_widget = new Cube();
        } else if(settings.shapeType == SHAPE_CONE){
            m_widget = new Cone();
        } else if(settings.shapeType == SHAPE_SPHERE){
            m_widget = new Sphere();
        } else if (settings.shapeType == SHAPE_CYLINDER){
            m_widget = new Cylinder();
        } else if (settings.shapeType == SHAPE_TORUS) {
            m_widget = new Torus();
        } else {
            std::cout << "That shape isn't implemented yet! Here's a cube" << std::endl;

            m_widget = new Quad();
        }
        m_widget->initializeShape(settings.shapeParameter1, settings.shapeParameter2);
        m_widget->draw();
    }
}

void ShapesScene::changeShapeParameter(){
    if(settings.shapeParameter1 != m_widget->getShapeParameterOne() || settings.shapeParameter2 != m_widget->getShapeParameterTwo()){
        m_widget->initializeShape(settings.shapeParameter1, settings.shapeParameter2);
        m_widget->draw();
    }
}
