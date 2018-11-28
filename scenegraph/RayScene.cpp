#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"

#include <iostream>
#include "shape/implicitshape.h"
#include "shape/implicitsphere.h"
#include "shape/implicitcone.h"
#include "shape/implicitcylinder.h"
#include "shape/implicitcube.h"
#include "glm/ext.hpp"

RayScene::RayScene(Scene &scene) :
    Scene(scene)
{
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.
    paintImplicitScene(scene);
}

RayScene::~RayScene()
{
}

glm::vec3 RayScene::recolorSceneRecursive(int width, int height, glm::mat4x4 camToWorld, glm::vec4 eye, glm::vec4 d, int recursionDepth, int primToSkip){
    glm::vec3 color = {0.0f,0.0f,0.0f};
    if(recursionDepth > 3){
        return color;
    }
    RayInfo rayInfo = getCollisionShape(eye, d, primToSkip);


    if(rayInfo.res[0] == INFINITY || rayInfo.res[1] == INFINITY || rayInfo.res[2] == INFINITY){
        return color;
    }
    glm::mat4x4 modelMat = rayInfo.modelMat;
    // Compute object space normal at object space intersection
    glm::vec3 normal = rayInfo.normal;

    glm::vec4 resVector = rayInfo.res;
    // Transform object space normal to world space
    glm::vec4 wsNormal = glm::vec4(glm::normalize(transformNormal(modelMat, normal)), 0.0f);

    //use normal and intersection point (both in WS) for lighting computation
    glm::vec3 ko = rayInfo.cAmbient; // should be a vec3
    glm::vec3 sumTerm = {0.0f, 0.0f, 0.0f};
    glm::vec3 oD = rayInfo.cDiffuse;
    glm::vec3 oS = rayInfo.cSpecular;
    glm::vec3 oR = rayInfo.cReflective;
    rayInfo.d = glm::normalize(d);

    int numLights = m_lightInfo.size();
    for(int i = 0; i < numLights; i++){


        if(!generateHardShadows(resVector, m_lightInfo[i], rayInfo.id)){
            continue;
        }

        // for all colors... (this should be a vector)
        glm::vec3 iM = {m_lightInfo[i]->color.r, m_lightInfo[i]->color.g, m_lightInfo[i]->color.b};

        glm::vec4 fromLight = m_lightInfo[i]->pos - resVector;
        if(m_lightInfo[i]->type ==  LightType::LIGHT_DIRECTIONAL){
            fromLight = -glm::normalize(m_lightInfo[i]->dir);
        }

        glm::vec4 n = wsNormal;

        glm::vec4 lM = (glm::normalize(fromLight));
        float norm = fmax(0.0f, (glm::dot(n, lM)));


        //texture
        if(rayInfo.textureImage != nullptr){
            // 1. read texture into QImage
            // 2. calculate u, v values and adjust for repeats
            // 3. get color at (r,c)
            // 4. blend
            QImage textureImage = *rayInfo.textureImage;
            glm::vec3 uv = rayInfo.uv;
            int row = ((int)(uv.x * rayInfo.texture.repeatU * textureImage.width())) % (textureImage.width());
            int col = ((int)(uv.y * rayInfo.texture.repeatV * textureImage.height())) % (textureImage.height());
            QColor color = textureImage.pixel(row, col);
            glm::vec3 texColor = {color.redF(), color.greenF(), color.blueF()};
            oD = rayInfo.blend * texColor + (1.0f-rayInfo.blend) * oD;
        }
        float rVal = iM[0] * (oD[0] * norm);
        float gVal = iM[1] * (oD[1] * norm);
        float bVal = iM[2] * (oD[2] * norm);
        glm::vec3 diffuse = glm::vec3(rVal, gVal, bVal);



        glm::vec4 pointToLight = lM;
        glm::vec4 pointToEye = glm::normalize(eye - resVector);
        glm::vec4 reflectBack = glm::normalize( 2.0f * glm::max(0.0f,(glm::dot(pointToLight, wsNormal))) * wsNormal - pointToLight);

        float rvD =  glm::pow(glm::max(0.0f, glm::dot(reflectBack, pointToEye)), rayInfo.shiny);
        if(rvD < 0.00000005f){
            rvD = 0.0f;
        }
        float specularR = glm::max(0.0f, iM[0] *(oS.x) * rvD);
        float specularG = glm::max(0.0f, iM[1] *(oS.y) * rvD);
        float specularB = glm::max(0.0f, iM[2] *(oS.z) * rvD);

        glm::vec3 specular = glm::vec3(specularR, specularG, specularB);
        float att = 1.0f;
        if(m_lightInfo[i]->type ==  LightType::LIGHT_DIRECTIONAL){
            att = 1.0f;
        } else {
            float mag = glm::distance(m_lightInfo[i]->pos, resVector);
            att = 1.0f / (m_lightInfo[i]->function.x + m_lightInfo[i]->function.y * mag + m_lightInfo[i]->function.z * mag * mag);
        }
        att = fmin(1.0f, att);

        sumTerm += (att * (diffuse + specular));
    }

    d = glm::normalize(d);
    glm::vec4 reflection = d + 2.0f * glm::max(0.0f,(glm::dot(-1.0f * d, wsNormal))) * wsNormal;

    glm::vec4 adjustment = {0.0001f, 0.0001f, 0.0001f, 0.0f};
    adjustment = modelMat * adjustment;
    glm::vec3 recursiveTerm = recolorSceneRecursive(width, height, camToWorld, resVector + adjustment, glm::normalize(reflection), recursionDepth + 1, rayInfo.id);
    float rVal =  glm::max(0.0f, recursiveTerm[0] * (oR[0] ));
    float gVal =  glm::max(0.0f, recursiveTerm[1] * (oR[1]));
    float bVal =  glm::max(0.0f, recursiveTerm[2] * (oR[2] ));
    glm::vec3 recursiveColor = glm::vec3(rVal, gVal, bVal);
    if(recursiveColor != glm::vec3()){
     //   std::cout << glm::to_string(recursiveColor) << std::endl;
    }

    glm::vec3 intensity = ko + sumTerm +  recursiveColor;
    intensity.x = fmax(fmin(intensity.x, 1.0f), 0.0f);
    intensity.y = fmax(fmin(intensity.y, 1.0f), 0.0f);
    intensity.z = fmax(fmin(intensity.z, 1.0f), 0.0f);
    //intensity = glm::min(intensity, glm::vec3(1.0f))

    return intensity;
}

bool RayScene::generateHardShadows(glm::vec4 surfacePoint, CS123SceneLightData *candidate, int primId){
    glm::vec4 p = glm::vec4(surfacePoint.x+0.0001f, surfacePoint.y+0.0001f, surfacePoint.z+0.0001f, 1.0f);
    glm::vec4 d = glm::vec4();
    if(candidate->type ==  LightType::LIGHT_DIRECTIONAL){
        d = glm::normalize(-candidate->dir);
    } else {
        d = glm::normalize(candidate->pos - p);
    }

    float bestT = INFINITY;
    PrimitiveInfo *bestObject;
    RayInfo rayInfo;
    rayInfo.res = {INFINITY, INFINITY, INFINITY, INFINITY};

    for(int prim = 0; prim < m_primitiveInfo.size(); prim++){
        PrimitiveInfo *currPrim = m_primitiveInfo[prim];
        if(currPrim->id == primId){
            continue;
        }

        float shapeIntersect = findObjectIntersection(*currPrim, p, d);
        if(shapeIntersect < INFINITY){
            return false;
        }
//        if(shapeIntersect < bestT){
//            bestT = shapeIntersect;
//            bestObject = currPrim;

//            rayInfo.intersect = shapeIntersect; //distance to point of intersection in object space

//            rayInfo.res = rayToObjectSpace(*bestObject, p) + shapeIntersect * rayToObjectSpace(*bestObject, d); //point of intersection in object space
//            //assert(rayInfo.res.w == 1.0);

//            rayInfo.normal = findObjectNormal(*bestObject, rayInfo.res.xyz()); // normal in object space.

//            rayInfo.res = p + shapeIntersect * d;// + glm::vec4(0.0001f, 0.0, 0.0, 0.0f); //point of intersection in world space

//            rayInfo.modelMat = bestObject->mat;


//            //CAUTION: we are discarding the alpha here.
//            rayInfo.cDiffuse = bestObject->material.cDiffuse.xyz();
//            rayInfo.cAmbient = bestObject->material.cAmbient.xyz();
//            rayInfo.cSpecular = bestObject->material.cSpecular.xyz();

//            rayInfo.cReflective = bestObject->material.cReflective.xyz();
//            rayInfo.d = constructDirectionVector(rayInfo.res, p); // light ray direction in world space

//            rayInfo.shiny = bestObject->material.shininess;
//        }
    }



//    if(rayInfo.res[0] == INFINITY || rayInfo.res[1] == INFINITY || rayInfo.res[2] == INFINITY){
//        return true;
//    }

    // Check if intersects with light.
    // Step 1: intersects with an object. Does it intersect with a light FIRST?
    // Step 2: doesn't intersect with an object. Does it intersect witha  light?
    return true;
}

std::vector<BGRA> RayScene::recolorScene(int width, int height, glm::mat4x4 camToWorld, glm::vec4 eye){

    //assert(eye.w == 1.0);
    int numLights = m_lightInfo.size();
    std::vector<BGRA> intensities(width * height);
    for(int row = 0; row < height; row ++){
        for( int col = 0; col < width; col ++){        
            // Select object with smallest non-negative t-value
            size_t centerIndex = (row * width + col) ;

            glm::vec4 worldRay = generatePrimaryRay(col, row, width, height, camToWorld);
            glm::vec4 d = constructDirectionVector(worldRay, eye);

            glm::vec3 intensity = recolorSceneRecursive(width, height, camToWorld, eye, d, 0, 0);
            BGRA color;
            color.r = (unsigned char)(intensity.x * 255.0f +0.5f);
            color.g = (unsigned char)(intensity.y * 255.0f + 0.5f);
            color.b = (unsigned char)((intensity.z * 255.0f + 0.5f));

            intensities[centerIndex] = (color);

        }
    }
    return intensities;
}
glm::vec3 RayScene::transformNormal(glm::mat4x4 modelMat, glm::vec3 normal){
    glm::mat3x3 tInv = glm::mat3x3(modelMat);
    return ((glm::transpose(glm::inverse(tInv))) * normal);
}
//primitive mat, cDiffuse
RayInfo RayScene::getCollisionShape(glm::vec4 p, glm::vec4 d, int primToSkip){
    float bestT = INFINITY;
    PrimitiveInfo *bestObject;
    RayInfo rayInfo;
    rayInfo.res = {INFINITY, INFINITY, INFINITY, INFINITY};

    for(int prim = 0; prim < m_primitiveInfo.size(); prim++){
        PrimitiveInfo *currPrim = m_primitiveInfo[prim];
        if(primToSkip == currPrim->id){
            continue;
        }
        float shapeIntersect = findObjectIntersection(*currPrim, p, d);

        if(shapeIntersect < bestT){
            bestT = shapeIntersect;
            bestObject = currPrim;

            rayInfo.intersect = shapeIntersect; //distance to point of intersection in object space

            rayInfo.res = rayToObjectSpace(*bestObject, p) + shapeIntersect * rayToObjectSpace(*bestObject, d); //point of intersection in object space
            //assert(rayInfo.res.w == 1.0);

            rayInfo.normal = findObjectNormal(*bestObject, rayInfo.res.xyz()); // normal in object space.

            rayInfo.res = p + shapeIntersect * d;// + glm::vec4(0.0001f, 0.0, 0.0, 0.0f); //point of intersection in world space

            rayInfo.modelMat = bestObject->mat;


            //CAUTION: we are discarding the alpha here.
            rayInfo.cDiffuse = bestObject->material.cDiffuse.xyz();
            rayInfo.cAmbient = bestObject->material.cAmbient.xyz();
            rayInfo.cSpecular = bestObject->material.cSpecular.xyz();
            rayInfo.id = bestObject->id;
            rayInfo.cReflective = bestObject->material.cReflective.xyz();
            rayInfo.d = constructDirectionVector(rayInfo.res, p); // light ray direction in world space
            rayInfo.texture = bestObject->material.textureMap;
            rayInfo.shiny = bestObject->material.shininess;

            rayInfo.textureImage = bestObject->textureImage;
            rayInfo.blend = bestObject->material.blend;
            glm::vec4 objSpacePoint = rayToObjectSpace(*bestObject, p) + shapeIntersect * rayToObjectSpace(*bestObject, d);
            rayInfo.uv = bestObject->imp->getTextureTarget(objSpacePoint.xyz());
        }
    }

    return rayInfo;
}

glm::vec4 RayScene::constructDirectionVector(glm::vec4 worldPoint, glm::vec4 eyePoint){
    //assert(worldPoint.w == 1.0);
    //assert(eyePoint.w == 1.0);
    return glm::normalize(worldPoint - eyePoint);
}

//transform 2D integer to untransformed WC point
glm::vec4 RayScene::generatePrimaryRay(int screenX, int screenY, int canvasX, int canvasY, glm::mat4x4 camToWorld){
    glm::vec4 point = screenToFilm(screenX, screenY, canvasX, canvasY);
    //assert(point.w == 1.0);
    return filmToWorld(point, camToWorld);
}

// checked.
glm::vec4 RayScene::screenToFilm(int screenX, int screenY, int canvasX, int canvasY){
    float ratioX = float(screenX + 0.5)/float(canvasX); // +0.5
    float ratioY = float(screenY + 0.5)/float(canvasY);
    float scaleX = ratioX * 2.0f - 1.0f;
    float scaleY = 1.0f - ratioY * 2.0f;
    return glm::vec4(scaleX, scaleY, -1.0f, 1.0f);
}

glm::vec4 RayScene::filmToWorld(glm::vec4 screen, glm::mat4x4 camToWorld){
    //assert(screen.w == 1.0);
    return camToWorld * screen;
}

glm::vec3 RayScene::findObjectNormal(PrimitiveInfo object, glm::vec3 point){
    ImplicitShape *implicit = object.imp;

    return implicit->getObjectNormal(point);
}

float RayScene::findObjectIntersection(PrimitiveInfo object, glm::vec4 p, glm::vec4 d){
    // world to obj
    glm::vec4 pTilda = rayToObjectSpace(object, p);
    glm::vec4 dTilda = rayToObjectSpace(object, d);
    ImplicitShape* implicit = object.imp;
   // std::cout << "d : "<< glm::to_string(d) << std::endl;
   // std::cout << "ptilda :" << glm::to_string(pTilda) << std::endl;
    float t = implicit->getIntersection(pTilda, dTilda);
    return t;
}

glm::vec4 RayScene::rayToObjectSpace(PrimitiveInfo object, glm::vec4 ray){

    glm::mat3x3 tInv = glm::mat3x3(object.mat);

    tInv = (glm::inverse(tInv));

    return glm::vec4(glm::inverse(object.mat) * (ray));
}

void RayScene::paintImplicitScene(Scene &scene){
    for(int i = 0; i < m_primitiveInfo.size(); i++){
        PrimitiveInfo *curr = m_primitiveInfo[i];
        switch(curr->type){
            case PrimitiveType::PRIMITIVE_CUBE:
                curr->imp = new ImplicitCube();
            break;
            case PrimitiveType::PRIMITIVE_CONE:
                std::cout << "making primitive cone" << std::endl;
                curr->imp = new ImplicitCone();
            break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                curr->imp = new ImplicitCylinder();
            break;
            default:
                curr->imp = new ImplicitSphere();
            break;
        }
    }
}
