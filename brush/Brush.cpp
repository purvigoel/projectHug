/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while scompleting the Brush assignment.
 */

#include "Brush.h"
#include "Canvas2D.h"
#include "math.h"
#include <iostream>
#include "Settings.h"

static unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush(BGRA color, int radius) :
    // Pro-tip: Initialize all variables in the initialization list
    m_color(color),
    m_radius(radius)
{
    // Pro-tip: By the time you get to the constructor body, all of the
    // member variables have already been initialized.
}


Brush::~Brush()
{
    // Pro-tip: The destructor (here) is where you free all the resources
    // you allocated during the lifetime of the class
    // Pro-tip: If you use std::unique_ptr or std::vector, you probabably
    // won't need to put anything in here.
}

int Brush::getAlpha() const {
    return m_color.a;
}

BGRA Brush::getBGRA() const {
    return m_color;
}

int Brush::getBlue() const {
    return m_color.b;
}

int Brush::getGreen() const {
    return m_color.g;
}

int Brush::getRadius() const {
    return m_radius;
}

int Brush::getRed() const {
    return m_color.r;
}

void Brush::setAlpha(int alpha) {
    m_color.a = alpha;
}

void Brush::setBGRA(const BGRA &bgra) {
    m_color = bgra;
}

void Brush::setBlue(int blue) {
    m_color.b = blue;
}

void Brush::setGreen(int green) {
    m_color.g = green;
}

void Brush::setRed(int red) {
    m_color.r = red;
}

void Brush::setRadius(int radius) {
    m_radius = radius;
    // Make a new mask if the radius is reset.
    makeMask();
}

void Brush::registerBrushDown(int x, int y, Canvas2D *canvas){
    initializeSeparateDrawingLayer(canvas);
}

void Brush::registerBrushUp(int x, int y, Canvas2D *canvas){
    //blendLayers(canvas->data(), m_separateDrawingLayer);
    m_separateDrawingLayer.clear();
    m_oldCanvas.clear();
}

void Brush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas, bool alphaBlending) {
    BGRA *pix = canvas->data();
    // Find the bounds of the square we're changing on the canvas
    int startX = std::max(0, mouseX - m_radius);
    int endX = std::min(mouseX + m_radius + 1, canvas->width());
    int startY = std::max(0, mouseY - m_radius);
    int endY = std::min(mouseY + m_radius + 1, canvas->height());

    int skipStartX = 0;
    int skipStartY = 0;
    // This is the case where some part of the mask is off screen
    if(startX == 0){
        skipStartX = m_radius - mouseX;
    }
    if(startY == 0){
        skipStartY = m_radius - mouseY;
    }
    // We ensure that the diameter is always odd.
    int maskDiameter = 2 * m_radius + 1;
    int pixelCount = 0;
 std::cout << "dragging" << std::endl;
    for(int row = startY; row < endY; row ++){
        for(int col = startX; col < endX; col++){
            int index = row * canvas->width() + col;
            // Make an adjustment for an out-of-bounds mask.
            int maskRow = row - startY + skipStartY;
            int maskCol = col - startX + skipStartX;
            // Find the corresponding mask pixel.
            pixelCount = maskRow * maskDiameter + maskCol;
            if(alphaBlending){

                handleTargetSquare(index, pixelCount, pix, alphaBlending);
            } else {
                m_separateDrawingLayer[index] = blendColor(pixelCount, m_separateDrawingLayer[index], true);
            }
        }
    }
    if(!alphaBlending){
        blendLayers(pix, m_separateDrawingLayer);
    }
    canvas->update();
}

void Brush::handleTargetSquare(int index, int pixelCount, BGRA * pix, bool alphaBlending) {
    pix[index] = blendColor(pixelCount, pix[index], false);
}

void Brush::blendLayers(BGRA *canvas, std::vector<BGRA> separateDrawingLayer){
    float alpha = getAlpha()/(255.0);
    for(int i = 0; i < separateDrawingLayer.size(); i++){
        canvas[i].r = separateDrawingLayer[i].r* alpha + m_oldCanvas[i].r * (1-alpha);
        canvas[i].g = separateDrawingLayer[i].g* alpha + m_oldCanvas[i].g * (1-alpha);
        canvas[i].b = separateDrawingLayer[i].b* alpha + m_oldCanvas[i].b * (1-alpha);
        canvas[i].a = 255.0;
    }
}

void Brush::initializeSeparateDrawingLayer(Canvas2D * canvas){
    m_separateDrawingLayer.resize(canvas->width() * canvas->height());
    m_oldCanvas.resize(canvas->width() * canvas->height());
    for(int i = 0; i < canvas->width() * canvas->height(); i++){
        m_separateDrawingLayer[i] = canvas->data()[i];
        m_oldCanvas[i] = canvas->data()[i];
    }
}

BGRA Brush::blendColor(int maskNum, BGRA canvasColor, bool useAlphaBlending){
    BGRA newColor;
    // Blend color as necessary. If we are doing the alpha blending fix, we're going to ignore
    // the alpha value.
    if(useAlphaBlending){

        float alpha = 1.0;
        float minusAlpha = 1.0;
        newColor.r = getRed()* m_mask[maskNum] + canvasColor.r * (1 - m_mask[maskNum]);
        newColor.g = getGreen()* m_mask[maskNum]+ canvasColor.g * (1 - m_mask[maskNum]);
        newColor.b = getBlue()* m_mask[maskNum]+ canvasColor.b * (1 - m_mask[maskNum]);
        newColor.a = getAlpha()/255.0;


    } else {
        float alpha = getAlpha()/255.0;
        float minusAlpha = getAlpha()/255.0;
        newColor.r = canvasColor.r * (1 - minusAlpha * m_mask[maskNum]) + (alpha * m_mask[maskNum] * getRed());
        newColor.g = canvasColor.g * (1 - minusAlpha * m_mask[maskNum]) + (alpha * m_mask[maskNum] * getGreen());
        newColor.b = canvasColor.b * (1 - minusAlpha * m_mask[maskNum]) + (alpha * m_mask[maskNum] * getBlue());
        newColor.a = 255.0;
    }
    return newColor;
}

// distance between two pixels
float Brush::distance(int x, int y, int x2, int y2) {
    return sqrt(float((x-x2)*(x-x2) + (y-y2) * (y-y2)));
}
