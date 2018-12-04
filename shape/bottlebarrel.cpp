#include "bottlebarrel.h"

BottleBarrel::BottleBarrel()
{

}

BottleBarrel::~BottleBarrel()
{

}
// A bottle barrel is defined by polar equations that we then translate
// to cartesian coordinates.
std::vector<float> BottleBarrel::buildShape(int numRows, int numCols){
    std::vector<float> points;
    float phiIncrement = 2.0 * M_PI / float(numRows);
    float thetaIncrement =2.0 * M_PI/ float(numCols);
    // We draw the "forward" section then the "backward" section
    for(float theta =  2.0 * M_PI ; theta >= (0.0 - thetaIncrement); theta -= thetaIncrement){
        for(float phi = 2.0 * M_PI; phi >= M_PI - phiIncrement; phi -= (phiIncrement)){

            float x = ( 3 + cos(theta/2)*sin(phi) - sin(theta/2)*sin(2*phi)) * cos(theta);
            float y = (3 + cos(theta/2) * sin(phi) - sin(theta/2) * sin(2*phi)) * sin(theta);
            float z = sin(theta/2)*sin(phi) + cos(theta/2) * sin(2*phi);

            points.push_back( x);
            points.push_back( z);
            points.push_back( y);
            float norm = pow(x*x+ y*y + z*z, 0.5);
            //normal
            points.push_back(x/norm);
            points.push_back(y/norm);
            points.push_back(z/norm);
            float oldTheta = theta;
            theta += thetaIncrement;
            x = ( 3 + cos(theta/2)*sin(phi) - sin(theta/2)*sin(2*phi)) * cos(theta);
            y = (3 + cos(theta/2) * sin(phi) - sin(theta/2) * sin(2*phi)) * sin(theta);
            z = sin(theta/2)*sin(phi) + cos(theta/2) * sin(2*phi);
            norm = pow(x*x+ y*y + z*z, 0.5);
            points.push_back( x);
            points.push_back( z);
            points.push_back(y);
            theta = oldTheta;
            //normal
            points.push_back(x/norm);
            points.push_back(y/norm);
            points.push_back(z/norm);

        }
    }
    for(float theta =  2.0 * M_PI; theta >= (0.0 - thetaIncrement); theta -= thetaIncrement){
        for(float phi = M_PI + phiIncrement; phi >= 0.0 - phiIncrement; phi -= (phiIncrement)){

            float x = ( 3 + cos(theta/2)*sin(phi) - sin(theta/2)*sin(2*phi)) * cos(theta);
            float y = (3 + cos(theta/2) * sin(phi) - sin(theta/2) * sin(2*phi)) * sin(theta);
            float z = sin(theta/2)*sin(phi) + cos(theta/2) * sin(2*phi);

            points.push_back( x);
            points.push_back( z);
            points.push_back( y);
            float norm = pow(x*x+ y*y + z*z, 0.5);
            //normal
            points.push_back(x/norm);
            points.push_back(y/norm);
            points.push_back(z/norm);
            float oldTheta = theta;
            theta -= thetaIncrement;
            x = ( 3 + cos(theta/2)*sin(phi) - sin(theta/2)*sin(2*phi)) * cos(theta);
            y = (3 + cos(theta/2) * sin(phi) - sin(theta/2) * sin(2*phi)) * sin(theta);
            z = sin(theta/2)*sin(phi) + cos(theta/2) * sin(2*phi);
            norm = pow(x*x+ y*y + z*z, 0.5);
            points.push_back( x);
            points.push_back( z);
            points.push_back(y);
            theta = oldTheta;
            //normal
            points.push_back(x/norm);
            points.push_back(y/norm);
            points.push_back(z/norm);

        }
    }
    return points;
}

