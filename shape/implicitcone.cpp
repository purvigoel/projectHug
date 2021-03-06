#include "implicitcone.h"
#include <iostream>
ImplicitCone::ImplicitCone()
{

}

ImplicitCone::~ImplicitCone()
{

}
float ImplicitCone::getIntersection(glm::vec4 p, glm::vec4 d){

    float A = d.x * d.x + d.z * d.z - 0.25 * d.y * d.y;
    float B = 2 * (d.x * p.x + d.z * p.z - 0.25 * p.y * d.y + 0.125 * d.y);
    float C = p.x * p.x + p.z * p.z - 0.25 * p.y * p.y + 0.25 * p.y - 0.0625;

    std::vector<float> ts = ImplicitShape::solveForT(A, B, C);



    // CHECK IF INTERSECTS PLANE
    if(ts.size() < 1){
        float capT = (0.5f + p.y) / (-1.0f * d.y);
         glm::vec4 option = p + capT * d;
        if(capT > 0 && fabs(option.y  + 0.5f) < 0.00001 && option.x * option.x + option.z * option.z <= 0.25f){
            std::cout << "here" << std::endl;
            return capT;
        }
        return INFINITY;
    } else if (ts.size() == 1){
        glm::vec4 option = p + ts[0] * d;
        if(ts[0] > 0 && fabs(option.y + 0.5) < 0.0001 && option.y > -0.5 && ts[0] < INFINITY){
            return ts[0];
        }
        return INFINITY;
    } else {
        glm::vec4 option1 = p + ts[0] * d;
        float bestOne = INFINITY;
        if(ts[0] > 0 && option1.y < 0.5 && option1.y > -0.5 && ts[0] < INFINITY){
            bestOne = ts[0];
        }
        glm::vec4 option2 = p + ts[1] * d;
        if(ts[1] > 0 && option2.y < 0.5 && option2.y > -0.5 && ts[1] < INFINITY){

            if(ts[1] < bestOne){
                bestOne = ts[1];
            }
        }
        // CAP time
        float capT = (-0.5f - p.y) / (d.y);
        glm::vec4 option = p + capT * d;
        if(capT >= 0 && fabs(option.y + 0.5) < 0.0001 && option.x * option.x + option.z * option.z - 0.25f <= 0.0f){

            if(bestOne > capT){
                bestOne = capT;
            }
        }

        return bestOne;
    }
    return 0.0;
}

glm::vec3 ImplicitCone::getObjectNormal(glm::vec3 p){
    float adjustX = p.x;

    if(p.x == 0 && p.y == 0 && p.z == 0){
        adjustX += 0.00001;
    }

    if(fabs(p.y + 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(0.0f,-1.0f,0.0f));
    }
    glm::vec3 norm = {2 * adjustX, 0.5 * (0.5 - p.y), 2.0 * p.z};

    return glm::normalize(norm);
}

glm::vec3 ImplicitCone::getTextureTarget(glm::vec3 point){
    //HANDLE CAP
    if(fabs(point.y + 0.5f) < 0.0001f){
        return (glm::vec3(point.x + 0.5f,1.0f - point.z + 0.5f,0.0f));
    }

    float v = 1.0f - point.y + 0.5f;
    float fraction = (atan2(-point.z, -point.x) + M_PI) / (2 * M_PI);

    return glm::vec3{1.0f - fraction  , v, 0.0f};
}
