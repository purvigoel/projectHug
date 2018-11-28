/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"


#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include "brush/SpecialBrush.h"

#include "brush/filter.h"
#include "brush/filteredge.h"
#include "brush/filteraverage.h"
#include "brush/filterscale.h"
#include "brush/filtermedian.h"

#include "glm/ext.hpp"
#include "math.h"
Canvas2D::Canvas2D() :
    // @TODO: Initialize any pointers in this class here.
    m_rayScene(nullptr)
{
    m_brush = new ConstantBrush(getColorFromSettings(), settings.brushRadius);
    settingsChanged();
}

Canvas2D::~Canvas2D()
{
    //if m_brush, delete it
}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);
}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************


void Canvas2D::mouseDown(int x, int y) {
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

    if(m_brush->getBrushType() == BRUSH_SMUDGE || m_brush->getBrushType() == BRUSH_SPECIAL_1) {
        m_brush->brushDown(x, y, this);
    }
    m_brush->registerBrushDown(x, y, this);
}

void Canvas2D::mouseDragged(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    m_brush->brushDragged(x, y, this, !settings.fixAlphaBlending);

}

void Canvas2D::mouseUp(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    if(m_brush->getBrushType() == BRUSH_SPECIAL_1){
        m_brush->brushUp(500,500,this);
    }
    m_brush->registerBrushUp(500,500,this);
}

BGRA Canvas2D::getColorFromSettings(){
    BGRA color;
    color.r = settings.brushColor.r;
    color.b = settings.brushColor.b;
    color.g = settings.brushColor.g;
    color.a = settings.brushColor.a;
    return color;
}

void Canvas2D::animateBrush(){
    m_brush->update(this);
}

void Canvas2D::newBrushType(){
    if(settings.brushType != m_brush->getBrushType()){
        delete m_brush;
        switch(settings.brushType){
            case BRUSH_CONSTANT:
                m_brush = new ConstantBrush(getColorFromSettings(), settings.brushRadius);
                break;
            case BRUSH_LINEAR:
                m_brush = new LinearBrush(getColorFromSettings(), settings.brushRadius);
                break;
            case BRUSH_QUADRATIC:
                m_brush = new QuadraticBrush(getColorFromSettings(), settings.brushRadius);
                break;
            case BRUSH_SMUDGE:
                m_brush = new SmudgeBrush(getColorFromSettings(), settings.brushRadius);
                break;
            case BRUSH_SPECIAL_1:
                m_brush = new SpecialBrush(getColorFromSettings(), settings.brushRadius);
                m_brush->createTimer(this, width(), height());
                break;
            default:
                m_brush = new ConstantBrush(getColorFromSettings(), settings.brushRadius);
                std::cout << "Sorry! That brush isn't implemented yet. Here's a constant one." << std::endl;
                break;
        }
    }
}

void Canvas2D::newBrushColor(){
    if(settings.brushColor.r != m_brush->getRed()){
        m_brush->setRed(settings.brushColor.r);
    }
    if(settings.brushColor.g != m_brush->getGreen()){
        m_brush->setGreen(settings.brushColor.g);
    }
    if(settings.brushColor.b != m_brush->getBlue()){
        m_brush->setBlue(settings.brushColor.b);
    }
    if(settings.brushColor.a != m_brush->getAlpha()){
        m_brush->setAlpha(settings.brushColor.a);
    }
}

void Canvas2D::newBrushRadius(){
    if(settings.brushRadius != m_brush->getRadius()){
        m_brush->setRadius(settings.brushRadius);
    }
}

// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.
    std::unique_ptr<Filter> filt;
    std::vector<BGRA> result;

    int xLo = 0;
    int yLo = 0;
    int xHi = 0;
    int yHi = 0;

    switch(settings.filterType) {
        case FILTER_BLUR:
            filt = std::make_unique<FilterAverage>(xLo, yLo, xHi, yHi);
            result = filt->applyFilter(this);
            paintCanvas(result);
            break;
        case FILTER_EDGE_DETECT:
             filt = std::make_unique<FilterEdge>(xLo, yLo, xHi, yHi);
             result = filt->applyFilter(this);
             paintCanvas(result);
            break;
        case FILTER_SPECIAL_1:
            filt = std::make_unique<FilterMedian>(xLo, yLo, xHi, yHi);
             result = filt->applyFilter(this);
            paintCanvas(result);
            break;
        case FILTER_SCALE:
            filt = std::make_unique<FilterScale>(xLo, yLo, xHi, yHi);
            result = filt->applyFilter(this);

        break;
            // fill in the rest
    }



    update();

}

void Canvas2D::paintCanvas(std::vector<BGRA> result){
    QPoint start = marqueeStart();
    QPoint end = marqueeStop();
    int xLo = start.x();
    int yLo = start.y();
    if(xLo < 0){
        xLo = 0;
    }
    if(yLo < 0){
        yLo = 0;
    }
    int xHi = end.x();
    int yHi = end.y();

    if(xHi < xLo){
        int temp = xLo;
        xLo = xHi;
        xHi = temp;
    }
    if(yHi < yLo){
        int temp = yLo;
        yLo = yHi;
        yHi = temp;
    }

    if(xHi > width()){
        xHi = width();
    }
    if(yHi > height()) {
        yHi = height();
    }

    if(xHi == xLo || yHi == yLo){
        xHi = width();
        xLo = 0;
        yHi = height();
        yLo = 0;
    }

    BGRA *data = this->data();
    int i = 0;
    for(int r = 0; r < height(); r ++){
        for(int c = 0; c<width(); c++){

            if( ( r <= yHi) && (r >= yLo) && (c <= xHi) && ( c >= xLo)){
                 data[i] = result[i];
            }
            i += 1;
        }
    }

}

void Canvas2D::setScene(RayScene *scene) {
    m_rayScene.reset(scene);
}

void Canvas2D::renderImage(Camera *camera, int width, int height) {
    std::cout << "width comparison" <<  width << " " << this->width() << std::endl;
    std::cout << "height comparison" <<  height << " " << this->height() << std::endl;
    if (m_rayScene) {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.
        this->resize(width, height);

        glm::mat4x4 cameraMat = glm::inverse(camera->getScaleMatrix() * camera->getViewMatrix());

        // glm::mat3x3 camToWorld = {cameraMat[0].xyz(), cameraMat[1].xyz(), cameraMat[2].xyz()};
       // std::cout << glm::to_string(camToWorld) << std::endl;


        glm::vec4 eye = glm::inverse(camera->getViewMatrix()) * glm::vec4(0,0,0,1);
        std::cout << "eye : " << glm::to_string(eye) << std::endl;
        std::cout << "cam : " << glm::to_string(cameraMat) << std::endl;

        std::vector<BGRA> newColors = m_rayScene->recolorScene(width, height, cameraMat, eye);
        //RayInfo test = m_rayScene->getCollisionShape(floor(width/2), floor(height/2), width, height, cameraMat, eye);

        BGRA *data = this->data();
        for(int i = 0; i < width * height; i++){
            data[i] = newColors[i];
        }

    }
}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}



void Canvas2D::settingsChanged() {
    // TODO: Process changes to the application settings.
    newBrushType();
    newBrushColor();
    newBrushRadius();
}
