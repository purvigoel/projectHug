#include "implicitcube.h"
#include "implicitshape.h"
#include "math.h"
#include <iostream>
#include "glm/ext.hpp"
ImplicitCube::ImplicitCube()
{

}

ImplicitCube::~ImplicitCube()
{

}
float ImplicitCube::getIntersection(glm::vec4 p, glm::vec4 d){

//    float aX = (-0.5f - p.x) / d.x;
//    float bX = (0.5f - p.x) / d.x;

//    float xMin = fmin(aX, bX);
//    float xMax = fmax(aX, bX);

//    float aY = (-0.5f - p.y) / d.y;
//    float bY = (0.5f - p.y)/d.y;

//    float yMin = fmin(aY, bY);
//    float yMax = fmax(aY, bY);

//    float aZ = (-0.5f - p.z) / d.z;
//    float bZ = (0.5f - p.z) / d.z;

//    float zMin = fmin(aZ, bZ);
//    float zMax = fmax(aZ, bZ);

//    if(xMin > yMax){
//        return INFINITY;
//    } else if (yMin > xMax){
//        return INFINITY;
//    }

//    xMin = fmax(xMin, yMin);
//    xMax = fmin(xMax, yMax);

//    if(xMin > zMax){
//        return INFINITY;
//    } else if (zMin > xMax){
//        return INFINITY;
//    }

//    xMin = fmax(xMin, zMin);
//    xMax = fmin(xMax, zMax);

//    float bestOne = fmin(xMin, xMax);
//    float capT = (-0.5f - p.y) / (d.y);
    float bestOne = INFINITY;
    float capT = (-0.5f - p.y) / (d.y);
    glm::vec4 option = p + capT * d;
    if(capT > 0 && fabs(option.y) < 0.5f + 0.00001 && fabs(option.x) < 0.5f + 0.00001
            && fabs(option.z) < 0.5f + 0.00001){
        if(bestOne > capT){
            bestOne = capT;
        }
    }

    capT = (0.5f - p.y) / (d.y);

    option = p + capT * d;
    if(capT > 0 && fabs(option.y) < 0.5f + 0.00001 && fabs(option.x) < 0.5f + 0.00001
            && fabs(option.z) < 0.5f + 0.00001){
        if(bestOne > capT){
            bestOne = capT;
        }
    }

    capT = (0.5f - p.x) / (d.x);
    option = p + capT * d;
    if(capT > 0 && fabs(option.y) < 0.5f + 0.00001 && fabs(option.x) < 0.5f + 0.00001
            && fabs(option.z) < 0.5f + 0.00001){
        if(bestOne > capT){
            bestOne = capT;
        }
    }

    capT = (-0.5f - p.x) / (d.x);

    option = p + capT * d;
    if(capT > 0 && fabs(option.y) < 0.5f + 0.00001 && fabs(option.x) < 0.5f + 0.00001
            && fabs(option.z) < 0.5f + 0.00001){
        if(bestOne > capT){
            bestOne = capT;
        }
    }
    capT = (-0.5f - p.z) / (d.z);
    option = p + capT * d;
    if(capT > 0 && fabs(option.y) < 0.5f + 0.00001 && fabs(option.x) < 0.5f + 0.00001
            && fabs(option.z) < 0.5f + 0.00001){
        if(bestOne > capT){
            bestOne = capT;
        }
    }

    capT = (0.5f - p.z) / (d.z);

    option = p + capT * d;
    if(capT > 0 && fabs(option.y) < 0.5f + 0.00001 && fabs(option.x) < 0.5f + 0.00001
            && fabs(option.z) < 0.5f + 0.00001){
        if(bestOne > capT){
            bestOne = capT;
        }
    }

    return bestOne;
}

glm::vec3 ImplicitCube::getObjectNormal(glm::vec3 p){
    float adjustX = p.x;

    if(p.x == 0 && p.y == 0 && p.z == 0){
        adjustX += 0.00001;
    }

    if(fabs(p.y + 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(0.0f,-1.0f,0.0f));
    }
    if(fabs(p.y - 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(0.0f,1.0f,0.0f));
    }
    glm::vec3 norm = {2 * adjustX, 0.5 * (0.5 - p.y), 2.0 * p.z};

    return glm::normalize(norm);
}

glm::vec3 ImplicitCube::getTextureTarget(glm::vec3 point){
    //HANDLE CAP
    if(fabs(point.y + 0.5f) < 0.001f){
        return (glm::vec3(point.x + 0.5f, - point.z + 0.5f,0.0f));
    }
    if(fabs(point.y - 0.5f) < 0.001f){
        return (glm::vec3(point.x+ 0.5f,0.5f + point.z ,0.0f));
    }
    if(fabs(point.x + 0.5f) < 0.001f){
        return (glm::vec3(point.z + 0.5f, - point.y + 0.5f,0.0f));
    }
    if(fabs(point.x - 0.5f) < 0.001f){
        return (glm::vec3( -point.z + 0.5f, 0.5f - point.y ,0.0f));
    }
    if(fabs(point.z + 0.5f) < 0.001f){
        return (glm::vec3(-(point.x - 0.5f), - point.y + 0.5f,0.0f));
    }
    if(fabs(point.z - 0.5f) < 0.001f){
        return (glm::vec3( point.x+ 0.5f,0.5f - point.y ,0.0f));
    }
    std::cout << glm::to_string(point) << std::endl;
    return glm::vec3{0, 0, 0.0f};
}
