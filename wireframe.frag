//#version 330 core

//in vec2 texCoord;

//uniform sampler2D tex;

//out vec4 fragColor;

//void main() {
//  //  fragColor = texture(tex, texCoord);
//    fragColor = vec4(1.0,0.0,1.0,1.0);
//}
#version 330 core

in vec4 cameraSpace;
in vec4 worldSpace;
in vec4 eye;
in vec3 color;
in vec2 texc;
in mat4x4 cam;
out vec4 fragColor;

uniform sampler2DRect tex;
uniform int useTexture = 0;
uniform vec2 repeatUV;
const int SPHERENUM = 4;
uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

uniform int timer = 2;

uniform vec2 resolution;
uniform float time;

const int raytraceDepth = 2;
const float M_PI = 3.1415926535897932384626433832795;

//void main(){
//    vec3 texColor = texture(tex, texc).rgb;
//    texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
//    //fragColor = vec4(color * texColor, 1);
//    fragColor = vec4(0.5,0.5,0.5, 1);
//}

float raySphereIntersect(vec3 r0, vec3 rd, vec3 s0, float sr) {
    // - r0: ray origin
    // - rd: normalized ray direction
    // - s0: sphere center
    // - sr: sphere radius
    // - Returns distance from r0 to first intersecion with sphere,
    //   or -1.0 if no intersection.

    //CONVERT r0 and rD to object space
    float a = dot(rd, rd);
    vec3 s0_r0 = r0 - s0;
    float b = 2.0 * dot(rd, s0_r0);
    float c = dot(s0_r0, s0_r0) - (sr * sr);
    if (b*b - 4.0*a*c < 0.0) {
        return -1.0;
    }
    float t1 = (-b - sqrt((b*b) - 4.0*a*c))/(2.0*a);
    float t2= (-b + sqrt((b*b) - 4.0*a*c))/(2.0*a);
    vec3 option1 = r0 + t1*rd;
    vec3 option2 = r0 + t2*rd;
    float best = 100;
    if(t1 > 0 && option1.y < 0.5 && option1.y > -0.5){
        best = t1;
    }
    if(t2 > 0 && option2.y < 0.5 && option2.y > -0.5 && t2 < best) {
        best = t2;
    }
    if(best == 100){
        best = -1;
    }
    return best;
}

//float rayTriangleIntersect(vec3 r0, vec3 rd, vec3 s0, float sr){

//    for(int i = 0; i < 9; i += 3){
//     //   vec3 currCenter = vec3(mesh[i]);

//    }
//}

vec3 sphereNormal(vec3 p){
    float adjustX = p.x;
    if(abs(p.x) < 0.000001 && abs(p.y) < 0.000001 && abs(p.z) < 0.000001){
        adjustX += 0.00001;
    }
    return normalize(vec3(2.0f * adjustX, 2.0f * p.y, 2.0f * p.z));
}

struct Sphere {
    vec3 sphereCenter;
    mat4x4 modelMat;
    int id;
};

struct IntersectInfo{
    int id;
    float t;
    mat4x4 modelMat;
};


mat4x4[SPHERENUM] makeSpheres(){
   mat4x4 spheres[SPHERENUM];
   mat4x4 transform = mat4(1.0f);

   for(int i = 0; i < 4; i ++){
       //vec4 pos = texture(tex, vec2(i,0));
       mat4x4 scale = mat4x4(1.0, 0.0, 0.0, 0.0,
                             0.0, 1.0, 0.0, 0.0,
                             0.0, 0.0, 1.0, 0.0,
                             0.0, 0.0, 0.0, 1.0);
       mat4x4 transform2 = mat4x4(1.0, 0.0 , 0.0,0.0,
                                  0.0, 1.0, 0.0,0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  (float(i) - 1.0f), sin(i + timer), 0.0f, 1.0);

      spheres[i] = transform2 * scale ;
   }


   //spheres[0]= transform;
  // spheres[1]= transform * transform2;
   return spheres;
}

IntersectInfo findCollision(mat4x4[SPHERENUM] transforms, int length, vec3 r0, vec3 rd, int fromID){
    float bestT = 1000000;
    IntersectInfo info = IntersectInfo(-1, -1, mat4x4(1.0f));

    for(int i = 0; i < length; i++){
        mat4x4 modelMat = transforms[i];
        if(fromID == i){
            continue;
        }
        float currT = raySphereIntersect((inverse(modelMat) * vec4(r0,1.0)).xyz, normalize((inverse(modelMat) * vec4(rd,0.0))).xyz, vec3(0,0,0), 0.5);

        if(currT != -1 && currT < bestT && (fromID != i)  ){
            bestT = currT;
            info.id = i;
            info.t = currT;
            info.modelMat = modelMat;
        }
    }
    return info;
}

float sdRoundCone( in vec3 p, in float r1, float r2, float h )
{
    vec2 q = vec2( length(p.xz), p.y );

    float b = (r1-r2)/h;
    float a = sqrt(1.0-b*b);
    float k = dot(q,vec2(-b,a));

    if( k < 0.0 ) return length(q) - r1;
    if( k > a*h ) return length(q-vec2(0.0,h)) - r2;

    return dot(q, vec2(a,b) ) - r1;
}

IntersectInfo findShadowCollision(Sphere[2] spheres, int length, vec3 r0, vec3 rd, int fromID){
    float bestT = 1000000;
    IntersectInfo info = IntersectInfo(-1, -1, mat4x4(1.0f));
    for(int i = 0; i < length; i++){
        Sphere currSphere;
        if(i == 0){
            currSphere = spheres[0];
        }
        if(i == 1){
            currSphere = spheres[1];
        }
        float currT = raySphereIntersect((inverse(currSphere.modelMat) * vec4(r0,1.0)).xyz, normalize((inverse(currSphere.modelMat) * vec4(rd,0.0)).xyz), currSphere.sphereCenter, 0.5);

        if(currT != -1  && (fromID != currSphere.id || abs(currT) > 1.01) ){
            bestT = currT;
            info.id = currSphere.id;
            info.t = currT;
            info.modelMat = currSphere.modelMat;
        }
    }
    return info;
}

vec4 screenToFilm(){
    float ratioX = float(gl_FragCoord.x + 0.5)/float(500); // +0.5
    float ratioY = float(gl_FragCoord.y + 0.5)/float(500); //canvas XY
    float scaleX = ratioX * 2.0f - 1.0f;
    float scaleY = 1.0f - ratioY * 2.0f;
    vec4 screen =  vec4(scaleX, scaleY, -1.0f, 1.0f);
    return cam * screen;
}

void main()
{
    fragColor = vec4(0.5,0.5,0.5, 1);
    float att = 1.0;

    mat4x4 spheres[SPHERENUM] = makeSpheres();

    float sphereRadius = 1.0;
    vec3 rayStart = eye.xyz;

    vec3 rayDir = normalize(worldSpace.xyz - eye.xyz);

    // light location
    vec3 light = vec3(0, 10 + sin(timer), 10 + cos(timer));


    vec3 lightColorConstant = vec3(1.0,1.0,1.0); // light color
    vec3 objectDiffuse = vec3(1.0,0.0,0.0); // red sphere
    vec3 objectSpecular = vec3(1,1,1);
    vec3 objectAmbient = vec3(0, 0, 0);
    float cReflective = 0.75;
    float shiny = 25;
    vec3 calcColor = vec3(0.0f);
    float globalKS = 0.5;
    float globalKD = 1;
    float frac = 1.0;
    int bounceID = -1;
    for(int bounce = 0; bounce < 2; bounce++){
        // generate primary ray

        vec3 lightColor = lightColorConstant;
        IntersectInfo info = findCollision(spheres, SPHERENUM, rayStart, rayDir, bounceID);

        float res = info.t;
        bounceID = info.id;
        if(res != -1){

            vec3 intersectAt = ((inverse(info.modelMat) * vec4(rayStart,1.0))).xyz + res * normalize((inverse(info.modelMat) * vec4(rayDir,0.0)).xyz);//point of intersection in object space
            vec3 intersectionWS = rayStart + res * rayDir; // world space

//           vec3 adjustedShadowP = intersectionWS + vec3(0.001f,0.001f,0.001f);
//            IntersectInfo shadow = findShadowCollision(spheres, 2, adjustedShadowP, normalize(light - adjustedShadowP), bounceID);
//            if(bounce > -1 && shadow.t != -1){
//            }

            mat3x3 tInv = mat3x3(info.modelMat);
            vec3 objectNormal = normalize((transpose(inverse(tInv)) * sphereNormal(intersectAt)));


            //fromLight vector
            vec3 fromLight = light - intersectionWS;
            vec3 fromLightDir = normalize(fromLight); // point to light
            vec3 normalWS = objectNormal; // already converted to world space

//            // CALCULATE DIFFUSE
            float norm = max(0.0f, dot(normalWS, fromLightDir));
            float rVal = globalKD * lightColor.x * (objectDiffuse.x * norm);
            float gVal = globalKD *lightColor.y * (objectDiffuse.y * norm);
            float bVal = globalKD *lightColor.z * (objectDiffuse.z * norm);
            vec3 diffuse = vec3(rVal, gVal, bVal);

            vec3 pointToEye = normalize(rayStart - intersectionWS);

            // CALCULATE SPECULAR
            vec3 reflectBack = -normalize( reflect(fromLightDir, normalWS));

            float rvD =  pow(max(0.0f, dot(reflectBack, pointToEye)), shiny);

            if(rvD < 0.00000005f){
                rvD = 0.0f;
            }

            float specularR =  max(0.0f, globalKS *lightColor.x *(objectSpecular.x) * rvD);
            float specularG =  max(0.0f, globalKS *lightColor.y *(objectSpecular.y) * rvD);
            float specularB =  max(0.0f, globalKS *lightColor.z *(objectSpecular.z) * rvD);

            vec3 specular = vec3(specularR, specularG, specularB);
            specular = clamp(specular, 0.0, 1.0);
          //  attenuation
            float att = 1.0;
            vec3 toAdd = frac * (objectAmbient + att * (diffuse + specular));
            toAdd = clamp(toAdd, 0.0f, 1.0f);
            calcColor = calcColor + toAdd;
            frac = frac * (cReflective) * globalKS;
            calcColor = clamp(calcColor, 0.0f, 1.0f);

            // REFLECTION
            rayStart = intersectionWS + vec3(0.0001,0.0001,0.0001);

            rayDir = normalize(reflect(rayDir,normalWS));

        } else {

            calcColor = calcColor + vec3(0.0f,0.0f,0.0f);

        }

    }
    fragColor = vec4(calcColor, 1.0f);

}
