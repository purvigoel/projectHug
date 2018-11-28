#include "spherebarrel.h"

SphereBarrel::SphereBarrel()
{

}

SphereBarrel::~SphereBarrel()
{

}
// A sphere barrel is defined by polar equations that we then translate
// to cartesian coordinates.
std::vector<float> SphereBarrel::buildShape(int numRows, int numCols){
    std::vector<float> points;
    float phiIncrement = 2.0 * M_PI / float(numRows);
    float thetaIncrement = M_PI/numCols;
    // note that we skip over a section defined by the cap. That part needs to be drawn backwards.
    for(float theta =  M_PI/2.0 - thetaIncrement; theta > -1 * M_PI/2; theta -= thetaIncrement){
        for(float phi = 2.0 * M_PI; phi > (0.0 - phiIncrement); phi -= (phiIncrement)){
            // some floating point number funky business.
            if( phi < 0.00001) {
               phi = 0.0;
            }

            float x = cos(theta) * cos(phi);
            float y = cos(theta) * sin(phi);
            float z = sin(theta);

            points.push_back(0.5 * x);
            points.push_back(0.5 * y);
            points.push_back(0.5 * z);
            float norm = pow(x*x+ y*y + z*z, 0.5);
            //normal
            points.push_back(x/norm);
            points.push_back(y/norm);
            points.push_back(z/norm);

            x = cos(theta + thetaIncrement) * cos(phi);
            y = cos(theta+ thetaIncrement) * sin(phi);
            z = sin(theta+ thetaIncrement);
            norm = pow(x*x+ y*y + z*z, 0.5);
            points.push_back(0.5 * x);
            points.push_back(0.5 * y);
            points.push_back(0.5 * z);

            //normal
            points.push_back(x/norm);
            points.push_back(y/norm);
            points.push_back(z/norm);

        }
    }

    return points;
}

