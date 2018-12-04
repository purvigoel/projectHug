#include "implicitsphere.h"
#include <iostream>
ImplicitSphere::ImplicitSphere()
{
}
ImplicitSphere::~ImplicitSphere()
{

}
float ImplicitSphere::getIntersection(glm::vec4 p, glm::vec4 d){

    float A = d.x * d.x + d.y * d.y + d.z * d.z;
    float B = 2 * (d.x * p.x + d.y * p.y + d.z * p.z);
    float C = p.x * p.x + p.y * p.y + p.z * p.z - 0.25;

    std::vector<float> ts = ImplicitShape::solveForT(A, B, C);
    if(ts.size() < 1){
        return INFINITY;
    } else if (ts.size() == 1){
        glm::vec4 option = p + ts[0] * d;
        if(ts[0] > 0 && option.y < 0.5+0.000001 && option.y+0.000001 > -0.5 && ts[0] < INFINITY){
            return ts[0];
        }
        return INFINITY;
    } else {
        glm::vec4 option1 = p + ts[0] * d;
        float bestOne = INFINITY;
        if(ts[0]>= 0 && option1.y < 0.5+0.000001 && option1.y + 0.000001 > -0.5 && ts[0] < INFINITY){
            bestOne = ts[0];
        }
        glm::vec4 option2 = p + ts[1] * d;
        if(ts[1] >= 0 && option2.y < 0.5+0.000001 && option2.y + 0.000001 > -0.5 && ts[1] < INFINITY){
            if(ts[1] < bestOne){
                bestOne = ts[1];
            }
        }

        return bestOne;
    }
    return 0.0;
}

glm::vec3 ImplicitSphere::getObjectNormal(glm::vec3 p){
    float adjustX = p.x;

    if(fabs(p.x) < 0.000001 && fabs(p.y) < 0.000001 && fabs(p.z) < 0.000001){
        adjustX += 0.1;
    }
    glm::vec3 norm = {2.0f * adjustX, 2.0f * p.y, 2.0f * p.z};

    return glm::normalize(norm);
}
glm::vec3 ImplicitSphere::getTextureTarget(glm::vec3 point){
    //HANDLE CAP.
    if(fabs(point.y + 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(0.0f,0.0f,0.0f));
    }
    if(fabs(point.y - 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(0.0f,1.0f,0.0f));
    }
    // some trig action, we'll find what fraction of pi the angle is to get the height,
    // then scale up to the texture's (max, min)
    float v = asin(point.y/0.5f)/M_PI + 0.5;

    float fraction = atan2(point.z, point.x) / (2 * M_PI);
    // this is to adjust for the negative part of atan2's domain
    if(point.z < 0.0f){
        fraction += M_PI;
    }
    return glm::vec3{fraction, v, 0.0f};
}
