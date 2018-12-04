#ifndef PRIMITIVEINFO_H
#define PRIMITIVEINFO_H

#include "CS123SceneData.h"
#include "shape/implicitshape.h"

struct PrimitiveInfo
{
    PrimitiveInfo() = default;

    PrimitiveType type = PrimitiveType::PRIMITIVE_CUBE;
    std::string meshfile = " ";     // Only applicable to meshes
    CS123SceneMaterial material;
    glm::mat4x4 mat = glm::mat4x4();
    ImplicitShape* imp;
    QImage *textureImage;
    int id;
    PrimitiveInfo(PrimitiveInfo &prim){
        type = prim.type;
        meshfile = prim.meshfile;
        material = prim.material;
        imp = prim.imp;
        mat = prim.mat;
        id = prim.id;
        textureImage = prim.textureImage;
    }

};
struct LightInfo
{
    int id;
    LightType type;

    CS123SceneColor color;
    glm::vec3 function;  // Attenuation function

    glm::vec4 pos;       // Not applicable to directional lights
    glm::vec4 dir;       // Not applicable to point lights

    float radius;        // Only applicable to spot lights
    float penumbra;      // Only applicable to spot lights
    float angle;         // Only applicable to spot lights

    float width, height;
};

#endif // PRIMITIVEINFO_H
