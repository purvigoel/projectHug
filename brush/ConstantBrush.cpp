/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"
#include <iostream>
#include "Settings.h"
ConstantBrush::ConstantBrush(BGRA color, int radius)
    : Brush(color, radius)
{
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}

void ConstantBrush::makeMask() {
    // make the diameter odd
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

// Constant mask values
float ConstantBrush::getMaskValue(int radius, float distanceFromRadius){
    return 1.0;
}

int ConstantBrush::getBrushType() {
    return BrushType::BRUSH_CONSTANT;
}
