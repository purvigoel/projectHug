#include "implicitcube.h"
#include "implicitshape.h"
#include "math.h"
#include <iostream>
ImplicitCube::ImplicitCube()
{

}

ImplicitCube::~ImplicitCube()
{

}
float ImplicitCube::getIntersection(glm::vec4 p, glm::vec4 d){

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
        adjustX += 0.1;
    }
    if(fabs(p.z + 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(0.0f,0.0f,-1.0f));
    }
    if(fabs(p.z - 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(0.0f,0.0f,1.0f));
    }
    if(fabs(p.y + 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(0.0f,-1.0f,0.0f));
    }
    if(fabs(p.y - 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(0.0f,1.0f,0.0f));
    }
    if(fabs(p.x + 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(-1.0f,0.0f,0.0f));
    }
    if(fabs(p.x - 0.5f) < 0.0001f){
        return glm::normalize(glm::vec3(1.0f,0.0f,0.0f));
    }

    // Ideally we should not get here. That would be awkward.
    return glm::normalize(glm::vec3(1.0f,0.0f,0.0f));
}
glm::vec3 ImplicitCube::getTextureTarget(glm::vec3 point){
    // brute force, best force
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
    return glm::vec3{0, 0, 0.0f};
}
