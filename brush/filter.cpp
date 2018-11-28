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

std::vector<float> Filter::convolve(std::vector<float> kernel, int kernelR, int kernelC, std::vector<float> in, int width, int height, bool transpose, bool threshold){

    std::vector<float> result(width * height , 0.0);
    std::cout << kernelR << " " << kernelC << std::endl;

    int radiusR = floor(kernelR/2);
    int radiusC = floor(kernelC / 2);

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            float red_acc = 0.0;
            size_t centerIndex = (r * width + c) ;

//            if(! (( r <= m_yHi) && (r >= m_yLo) && (c <= m_xHi) && ( c >= m_xLo))){
//                red_acc = in[centerIndex];
//            } else {
                //std::cout << "HERE" << std::endl;
                for( int ki = 0; ki < kernelR; ki ++){ //3 vs 1
                    for(int kj = 0; kj < kernelC; kj ++){ //1 vs 3
                        float redVal = 0.0;
                        if (!( (r + ki - radiusR) < 0 || (kj + c - radiusC) < 0 || (r + ki - radiusR) >= height || (kj + c - radiusC) >= width )){
                            int imageC = (r + ki - radiusR) * width + (kj + c - radiusC);

                            redVal =  in[imageC];
                        }
                        int kernelCoord = ki * kernelC + kj;
                        float kernelVal = (kernel[kernelCoord]);

                        red_acc += (redVal * kernelVal);
                    }
                }
          //  }
            result[centerIndex] =  (red_acc);

            }
        }
        return result;
}


inline unsigned char Filter::REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}
