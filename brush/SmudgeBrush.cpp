/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"

#include "Canvas2D.h"
#include <iostream>
#include "Settings.h"
SmudgeBrush::SmudgeBrush(BGRA color, int radius) :
    QuadraticBrush(color, radius)
{
    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
}

void SmudgeBrush::brushUp(int x, int y, Canvas2D* canvas) {
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask() {
    int diameter = m_radius * 2 + 1;
    int vectorLength = diameter*diameter;
    std::vector<float> vectorMask(vectorLength, 0.0);
    for(int i = 0; i < vectorLength; i++) {
        int row = (i / diameter);
        int col = (i % diameter);
        float radiusDistance = distance(row, col, m_radius, m_radius);
        if(radiusDistance < m_radius){
            vectorMask[i] = getMaskValue(m_radius, radiusDistance);
        }
    }
    m_mask = vectorMask;
}

// I'm using a quadratic distribution here.
float SmudgeBrush::getMaskValue(int radius, float distanceFromRadius){
    return (1.0 - distanceFromRadius/radius) * (1.0 - distanceFromRadius/radius);
}

int SmudgeBrush::getBrushType(){
    return BrushType::BRUSH_SMUDGE;
}

void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas) {
    pickUpPaint(x, y, canvas);
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas) {
    BGRA *pix = canvas->data();
    int startX = std::max(0, x - m_radius);
    int endX = std::min(x + m_radius + 1, canvas->height());
    int startY = std::max(0, y - m_radius);
    int endY = std::min(y + m_radius + 1, canvas->width());

    int skipStartX = 0;
    int skipStartY = 0;

    if(startX == 0){
        skipStartX = m_radius - x;
    }
    if(startY == 0){
        skipStartY = m_radius - y;
    }
    int maskDiameter = 2 * m_radius + 1;
    std::vector<BGRA> tempBufferLocal(maskDiameter * maskDiameter);
    int index = 0;
    // Fill the temporary buffer with 0s.
    for(int row = 0; row < skipStartY; row ++){
        for(int col = 0; col < skipStartX; col++){
            index = (row * maskDiameter + col) ;
            tempBufferLocal[index].r = 0.0;
            tempBufferLocal[index].g = 0.0;
            tempBufferLocal[index].b = 0.0;
            tempBufferLocal[index].a = 0.0;
        }
    }
    // Fill the temporary buffer with the target square.
    for(int row = startY; row < endY; row ++){
        for(int col = startX; col < endX; col++){
            int maskRow = row - startY + skipStartY;
            int maskCol = col - startX + skipStartX;
            index = (maskRow * maskDiameter + maskCol) ;
            int canvasIndex = row * canvas->width() + col;
            tempBufferLocal[index].r = pix[canvasIndex].r;
            tempBufferLocal[index].g = pix[canvasIndex].g;
            tempBufferLocal[index].b = pix[canvasIndex].b;
            tempBufferLocal[index].a = pix[canvasIndex].a;
        }
    }
    m_tempBuffer = tempBufferLocal;
}

void SmudgeBrush::brushDragged(int x, int y, Canvas2D* canvas, bool alphaBlending) {
    // now pick up paint again...
    BGRA *pix = canvas->data();
    int startX = std::max(0, x - m_radius);
    int endX = std::min(x + m_radius + 1, canvas->height());
    int startY = std::max(0, y - m_radius);
    int endY = std::min(y + m_radius + 1, canvas->width());

    int skipStartX = 0;
    int skipStartY = 0;

    if(startX == 0){
        skipStartX = m_radius - x;
    }
    if(startY == 0){
        skipStartY = m_radius - y;
    }
    int maskDiameter = 2 * m_radius + 1;
    int pixelCount = 0;

    for(int row = startY; row < endY; row ++){
        for(int col = startX; col < endX; col++){
            int index = row * canvas->width() + col;
            int maskRow = row - startY + skipStartY;
            int maskCol = col - startX + skipStartX;
            pixelCount = maskRow * maskDiameter + maskCol;
            float red = m_tempBuffer[pixelCount].r;
            float green = m_tempBuffer[pixelCount].g;
            float blue = m_tempBuffer[pixelCount].b;
            float alpha = m_tempBuffer[pixelCount].a;
            pix[index] = smudgeColor(pixelCount,pix[index],red,green,blue,alpha);
        }
    }
    canvas->update();
    m_tempBuffer.clear();
    pickUpPaint(x, y, canvas);
}

BGRA SmudgeBrush::smudgeColor(int maskNum, BGRA oldColor, float red, float green, float blue, float a){
    BGRA newColor;
    float alpha = getAlpha()/255.0;
    newColor.r = oldColor.r * (1 - alpha * m_mask[maskNum]) + (alpha * m_mask[maskNum] * red);
    newColor.g = oldColor.g * (1 - alpha * m_mask[maskNum]) + (alpha * m_mask[maskNum] * green);
    newColor.b = oldColor.b * (1 - alpha * m_mask[maskNum]) + (alpha * m_mask[maskNum] * blue);
    newColor.a = 255.0;
    return newColor;
}

