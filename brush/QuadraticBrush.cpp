/**

 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <math.h>
#include "Settings.h"
QuadraticBrush::QuadraticBrush(BGRA color, int radius)
    : Brush(color, radius)
{
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}

void QuadraticBrush::makeMask() {
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

// quadratic distribution values
float QuadraticBrush::getMaskValue(int radius, float distanceFromRadius){
    return (1.0 - distanceFromRadius/radius) * (1.0 - distanceFromRadius/radius);
}

int QuadraticBrush::getBrushType() {
     return BrushType::BRUSH_QUADRATIC;
}
