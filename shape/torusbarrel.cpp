#include "torusbarrel.h"

TorusBarrel::TorusBarrel()
{

}

TorusBarrel::~TorusBarrel()
{

}

std::vector<float> TorusBarrel::buildShape(int numRows, int numCols){
    std::vector<float> points;
    float phiIncrement = 2 * M_PI / numRows;
    float thetaIncrement = 2 * M_PI/numCols;
    // Plug and chug into an equation.
    for(float theta =  M_PI/2; theta >= -3 * M_PI/2 - thetaIncrement; theta -= thetaIncrement){
        for(float phi = 2 * M_PI; phi >= 0 - phiIncrement; phi -= (phiIncrement)){
            float x = (1 + 0.5 * cos(theta)) * cos(phi);
            float y = (1 + 0.5 * cos(theta)) * sin(phi);
            float z = 0.5 * sin(theta);

            points.push_back( x);
            points.push_back( y);
            points.push_back( z);

            float norm = pow(x*x + y*y + z*z, 0.5);
            //normal
            points.push_back(cos(theta) * cos(phi)/norm);
            points.push_back(cos(theta) * sin(phi)/norm);
            points.push_back(sin(theta)/norm);

            x = (1 + 0.5 *cos(theta + thetaIncrement)) * cos(phi);
            y = (1 + 0.5 *cos(theta + thetaIncrement)) * sin(phi);
            z = 0.5 * sin(theta + thetaIncrement);

            points.push_back( x);
            points.push_back(y);
            points.push_back( z);
            norm = pow(x*x + y*y + z*z, 0.5);
            //normal
            points.push_back(cos(theta + thetaIncrement) * cos(phi)/norm);
            points.push_back(cos(theta + thetaIncrement) * sin(phi)/norm);
            points.push_back(sin(theta + thetaIncrement)/norm);

        }
    }
    return points;
}
