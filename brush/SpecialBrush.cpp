/**
 * @file   SpecialBrush.cpp
 *
 * Implementation of a brush with a special mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "SpecialBrush.h"
#include <iostream>
#include "Canvas2D.h"
#include <unistd.h>
#include <QTimer>
#include "Settings.h"
class Canvas2D;
SpecialBrush::SpecialBrush(BGRA color, int radius)
    : Brush(color, radius)
{
    makeMask();
}


SpecialBrush::~SpecialBrush()
{
    // delete the timer if it exists
    if(m_timer){
        delete m_timer;
    }
}
// create a noise field with the right height and width, and set up a timer for the animation
void SpecialBrush::createTimer(Canvas2D * canvas, int width, int height){
    noise(width,height);
    m_timer = new QTimer(canvas);
    QObject::connect(m_timer, SIGNAL(timeout()), canvas, SLOT(animateBrush()) );
}

// This is called with every loop of the timer
void SpecialBrush::update(Canvas2D *canvas){
    // First, redraw the initial circles to keep the fire going
    for(int x = 0; x < m_xs.size(); x++){
        Brush::brushDragged(m_xs[x], m_ys[x], canvas, true);
    }
    BGRA * pixOld = canvas->data();
    int canvasWidth = canvas->width();
    int canvasHeight = canvas->height();
    std::vector<BGRA> pix(canvasWidth * canvasHeight);
    // We're going to be calculating new values for the entire canvas
    for (int row = 1; row < canvasWidth - 1; row++){
        for(int col = 1; col<canvasHeight - 1; col++){
            int curr = row * canvasWidth + col;
            // Find the neighbors of the current square
            int neighbor1 = (row + 1) * canvasWidth + col;
            int neighbor2 = (row - 1) * canvasWidth + col;
            int neighbor3 = row * canvasWidth + (col - 1);
            int neighbor4 = row * canvasWidth + (col + 1);
            // Average the neighbors, and subtract the noise from them.
            float noiseReading = m_noise[curr];
            float averageR = (pixOld[neighbor1].r + pixOld[neighbor2].r + pixOld[neighbor3].r+pixOld[neighbor4].r)/4.0 - noiseReading;
            float averageG = (pixOld[neighbor1].g + pixOld[neighbor2].g + pixOld[neighbor3].g+pixOld[neighbor4].g)/4.0- noiseReading;
            float averageB = (pixOld[neighbor1].b + pixOld[neighbor2].b + pixOld[neighbor3].b+pixOld[neighbor4].b)/4.0- noiseReading;
            // No negative rgb values in this house.
            if( averageR < 0.0){
                averageR = 0.0;
            }
            if( averageB < 0.0){
                averageB = 0.0;
            }
            if( averageG < 0.0){
                averageG = 0.0;
            }
            // reset the canvas values. the calculated value will go one above the current value
            // because the fire is rising.
            pix[neighbor2].r = averageR;
            pix[neighbor2].g = averageG;
            pix[neighbor2].b = averageB;
            pix[neighbor2].a = 255.0;
        }
    }
    // With calculation complete, we copy the results onto our canvas.
    for(int i = 0; i < canvasWidth * canvasHeight ; i++){
        pixOld[i] = pix[i];
    }
    // we scroll the noise array up as well, to rise with the fire.
    for(int i = 0; i < canvasWidth; i ++){
        m_noise.push_back(m_noise[i]);
    }
    m_noise.erase(m_noise.begin(), m_noise.begin()+canvasWidth);
    canvas->repaint();
    usleep(10000);
}

// This should stop and restart the timer if its already running.
void SpecialBrush::startTimer(Canvas2D *canvas){
    m_timer->start(10);
}

void SpecialBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...
    int diameter = m_radius * 2 + 1;
    int vectorLength = diameter*diameter;
    std::vector<float> vectorMask(vectorLength, 0.0);
    for(int i = 0; i < vectorLength; i++) {
        int row = (i / diameter);
        int col = (i % diameter);
        float dist = distance(row, col, m_radius, m_radius);
        if( dist <= m_radius){
            vectorMask[i] = getMaskValue(row, col);
        }
    }
    m_mask = vectorMask;
}

// We'll just use a simple constant mask here.
float SpecialBrush::getMaskValue(int radius, float distanceFromRadius){
    return 1.0;
}

int SpecialBrush::getBrushType() {
    return BrushType::BRUSH_SPECIAL_1;
}

// Reset all values when the brush is down.
void SpecialBrush::brushDown(int x, int y, Canvas2D *canvas){
   m_shouldAnimate = false;
   m_xs.clear();
   m_ys.clear();
   m_xs.push_back(x);
   m_ys.push_back(y);
}

// Take note of all initial x/y brush movements.
void SpecialBrush::brushDragged(int x, int y, Canvas2D *canvas, bool alphaBlending){
   Brush::brushDragged(x, y, canvas, alphaBlending);
   m_xs.push_back(x);
   m_ys.push_back(y);
   m_shouldAnimate = true;
}

// This is the noise array that will give our fire some randomness.
std::vector<float> SpecialBrush::noise(int canvasWidth, int canvasHeight){
    int canvasArea = canvasWidth * canvasHeight;
    std::vector<float> noiseHolder(canvasArea, 0.0);
    // We choose a random number and scale it between 0 and a tuned number.
    for(int i = 0; i < canvasArea; i++){
       float r = (float)rand()/RAND_MAX * 40.0;
       if(r < 25.0){
           r = 0.0;
       }
       noiseHolder[i] = r;
    }
    // We're going to soften this noise so it looks less harsh by averaging
    // every pixel with its neighbors 5 times.
    for(int counter = 0; counter < 5; counter++){
        for (int row = 2; row < canvasHeight - 2; row++){
            for(int col = 2; col<canvasWidth - 2; col++){
                int curr = row * canvasWidth + col;
                int neighbor1 = (row + 1) * canvasWidth + col;
                int neighbor2 = (row - 1) * canvasWidth + col;
                int neighbor3 = row * canvasWidth + (col - 1);
                int neighbor4 = row * canvasWidth + (col + 1);
                noiseHolder[curr] = (noiseHolder[neighbor1]+noiseHolder[neighbor2]+noiseHolder[neighbor3]+noiseHolder[neighbor4]+noiseHolder[curr])*0.2;
            }
        }
    }
    // We'll dampen some of the values.
    for(int i = 0; i < canvasArea; i++){
       if(noiseHolder[i] < 5.0){
           noiseHolder[i] = 0.0;
       }
    }
    m_noise = noiseHolder;
    return m_noise;
}

void SpecialBrush::brushUp(int x, int y, Canvas2D *canvas){
    // brush is up, time to animate!
    startTimer(canvas);
}

