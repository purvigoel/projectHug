#include "filter.h"
#include "Canvas2D.h"
#include "math.h"
#include <iostream>
Filter::Filter(int xLo, int yLo, int xHi, int yHi)
{
    m_xHi = xHi;
    m_xLo = xLo;
    m_yLo = yLo;
    m_yHi = yHi;
}

Filter::~Filter(){

}
// Convolve the canvas with the given kernel
std::vector<float> Filter::convolve(std::vector<float> kernel, int kernelR, int kernelC, std::vector<float> in, int width, int height, bool transpose, bool threshold){

    std::vector<float> result(width * height , 0.0);
    for(int i= 0; i < in.size(); i ++){
        result[i] = in[i];
    }
    int radiusR = floor(double(kernelR)/2.0);
    int radiusC = floor(double(kernelC) / 2.0);
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = (r * width + c) ;

            float red_acc = 0.0;

            for( int ki = 0; ki < kernelR; ki ++){
                for(int kj = 0; kj < kernelC; kj ++){
                    float redVal = 0.0;
                    if (!( (r + ki - radiusR) < 0 || (kj + c - radiusC) < 0 || (r + ki - radiusR) >= height || (kj + c - radiusC) >= width )){
                        int imageC = (r + ki - radiusR) * width + (kj + c - radiusC);
                        // find the value here, after bounds checking
                        redVal =  in[imageC];
                    } else {
                        continue;
                    }
                    int kernelCoord = ki * kernelC + kj;
                    float kernelVal = float(kernel[kernelCoord]);
                    red_acc += float(redVal * kernelVal);
                }
            }
            result[centerIndex] =  (red_acc);
            }
        }
        return result;
}


inline unsigned char Filter::REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}
