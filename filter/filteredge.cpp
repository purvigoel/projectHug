#include "filteredge.h"
#include "filter.h"
#include <string.h>
#include "Settings.h"
#include "math.h"
#include <iostream>
FilterEdge::FilterEdge(int xLo, int yLo,int xHi,int yHi)
: Filter(xLo, yLo, xHi, yHi)
{
    m_rowKernel = std::vector<float>{1.0, 0.0, -1.0};
    m_colKernel = std::vector<float>{1.0,2.0,1.0};
}

FilterEdge::~FilterEdge()
{

}

std::vector<BGRA> FilterEdge::applyFilter(Canvas2D * canvas){
    std::vector<float> start(canvas->width() * canvas->height(), 0.0);

    BGRA* data = canvas->data();
    // Convert to gray first.
    for(int i = 0; i < canvas->width() * canvas->height(); i++){
        start[i] = float(data[i].r + data[i].g + data[i].b) / 3.0;
    }
    for(int i = 0; i < start.size(); i++){
        start[i] = start[i] / 255.0;
    }
    // Convolve in the x direction, then the y direction
    std::vector<float> step1 = Filter::convolve(m_rowKernel, 1, 3, start, canvas->width(), canvas->height(), false, true);
    std::vector<float> gx = Filter::convolve(m_colKernel, 3, 1, step1, canvas->width(), canvas->height(),false, true);

    std::vector<float> step2 = Filter::convolve(m_colKernel, 1, 3, start, canvas->width(), canvas->height(),false, true);
    std::vector<float> gy = Filter::convolve(m_rowKernel, 3, 1, step2, canvas->width(), canvas->height(),false, true);

    return gradientMagnitude(gx, gy);
}

// Threshholding
std::vector<BGRA> FilterEdge::gradientMagnitude(std::vector<float> x, std::vector<float> y){
    std::vector<BGRA> out;
    for(int i = 0; i < x.size(); i ++){
        BGRA col;
        float gxr = x[i] * 255.0 + 0.5;

        float gyr = y[i] * 255.0 + 0.5;

        float r = (sqrt(gxr * gxr + gyr * gyr));
        r = r * settings.edgeDetectSensitivity;

        if(r < 0){
            r = 0;
        }

        if (r > 255){
            r = 255;
        }
        col.r = r;
        col.g = r;
        col.b = r;
        out.push_back( col);
    }
    return out;
}

inline unsigned char FilterEdge::REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}
