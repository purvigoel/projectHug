#version 330 core

in vec4 cameraSpace;
in vec4 worldSpace;
in vec4 eye;
in vec3 color;
in vec2 texc;
in mat4x4 cam;
out vec4 fragColor;
out vec4 rayDir_data;

uniform sampler2DRect tex;
uniform int useTexture = 0;
uniform vec2 repeatUV;
const int SPHERENUM = 18;
uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

uniform int renderDir = 0;

uniform int bumpMapDemo = 0;

uniform samplerCube skybox;
uniform sampler2D bumpMapTex;

uniform int timer = 2;

uniform vec2 resolution;
uniform float time;

const int raytraceDepth = 2;
const float M_PI = 3.1415926535897932384626433832795;

bool intersected = false;
vec3 rayDir = vec3(0.0);

//void main(){
//    vec3 texColor = texture(tex, texc).rgb;
//    texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
//    //fragColor = vec4(color * texColor, 1);
//    fragColor = vec4(0.5,0.5,0.5, 1);
//}

float raySphereIntersect(vec3 origin, vec3 dir, float radius) {
    //CONVERT r0 and rD to object space
    float a = dot(dir, dir);
    float b = 2.0 * dot(dir, origin);
    float c = dot(origin, origin) - (radius * radius);
    if (b*b - 4.0*a*c < 0.0) {
        return -1.0;
    }
    float t1 = (-b - sqrt((b*b) - 4.0*a*c))/(2.0*a);
    float t2= (-b + sqrt((b*b) - 4.0*a*c))/(2.0*a);
    vec3 option1 = origin + t1*dir;
    vec3 option2 = origin + t2*dir;
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


vec3 sphereNormal(vec3 p){
    float adjustX = p.x;
    if(abs(p.x) < 0.000001 && abs(p.y) < 0.000001 && abs(p.z) < 0.000001){
        adjustX += 0.00001;
    }
    vec3 texSample = vec3(texture(bumpMapTex, p.xy));

//    if(bumpMapDemo == 1){
//        return normalize(vec3(texSample.x*2-1, texSample.y*2-1, texSample.z*2-1));
//    } else {
        return normalize(vec3(2.0f * adjustX, 2.0f * p.y, 2.0f * p.z));
// //   }
 //  return normalize(vec3(texSample.x*2-1, texSample.y*2-1, texSample.z*2-1));
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


       mat4x4 neckScale = mat4x4(0.5, 0.0, 0.0, 0.0,
                             0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.0,
                             0.0, 0.0, 0.0, 1.0);
       mat4x4 transform2 = mat4x4(1.0, 0.0 , 0.0,0.0,
                                  0.0, 1.0, 0.0,0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  (3.2f + (sin(float(timer ) + 1.0f)) / 1), -2.6f + (sin(float(timer ) + 1.0f)) / 10.0, 0f, 1.0);

      spheres[0] = transform2 * neckScale ;

      transform2 = mat4x4(1.0, 0.0 , 0.0,0.0,
                                    0.0,1.0, 0.0,0.0,
                                    0.0, 0.0, 1.0, 0.0,
                                    ( 0.5f), -1.0f, 0.0f, 1.0);


      mat4x4 bodyScale = mat4x4(2.0 + (sin(float(timer)) / 10.0f), 0.0, 0.0, 0.0,
                               0.0, 2.0 + (sin(float(timer))/ 10.0f), 0.0, 0.0,
                               0.0, 0.0, 2.0 + (sin(float(timer))/ 10.0f), 0.0,
                               0.0, 0.0, 0.0, 1.0);
       spheres[1]= transform2 * bodyScale;

       transform2 = mat4x4(1.0, 0.0 , 0.0,0.0,
                                     0.0, 1.0, 0.0,0.0,
                                     0.0, 0.0, 1.0, 0.0,
                                     (2.7f), -0.75f + sin(float(timer) + 1.0f)/20.0, 0.0f, 1.0);

        mat4x4 backscale =mat4x4(1.25, 0.0, 0.0, 0.0,
                             0.0, 1.25, 0.0, 0.0,
                             0.0, 0.0, 1.0, 0.0,
                             0.0, 0.0, 0.0, 1.0);
        spheres[2]= transform2 * backscale;

        mat4x4 headScale = mat4x4(1.1, 0.0, 0.0, 0.0,
                                 0.0, 1.1, 0.0, 0.0,
                                 0.0, 0.0, 1.0, 0.0,
                                 0.0, 0.0, 0.0, 1.0);
        float headTimer = (4 % 5)/5.0f;
        mat4x4 headRotate = mat4x4(cos(float(headTimer)), sin(float(headTimer)), 0.0f, 0.0f,
                                   -sin(float(headTimer)), cos(float(headTimer)), 0.0f, 0.0f,
                                   0.0f, 0.0f ,1.0f,0.0f,
                                   0.0f, 0.0f, 0.0f, 1.0f);
        transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                      0.0, 1.0, 0.0,0.0,
                                      0.0, 0.0, 1.0, 0.0,
                                      (- 1.4f), -0.3f, 0.0f, 1.0);


         spheres[3]= transform2 * headRotate * headScale;


         mat4x4 kneeScale = mat4x4(0.5, 0.0, 0.0, 0.0,
                                  0.0, 0.5, 0.0, 0.0,
                                  0.0, 0.0, 0.5, 0.0,
                                  0.0, 0.0, 0.0, 1.0);
         transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                       0.0,1, 0.0,0.0,
                                       0.0, 0.0, 1, 0.0,
                                       (0.5f + (sin(float(timer)))/2.0), -2.3f, 0f, 1.0);


          spheres[4]= transform2 * kneeScale;
          transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                        0.0,1, 0.0,0.0,
                                        0.0, 0.0, 1, 0.0,
                                        (3.0f + (sin(float(timer ) + 1.0f)) / 2.0), -1.6f + (sin(float(timer ) + 1.0f)) / 10.0, 0f, 1.0);


           spheres[5]= transform2 * kneeScale;

           transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                         0.0,1, 0.0,0.0,
                                         0.0, 0.0, 1, 0.0,
                                         (3.1f + (sin(float(timer ) + 1.0f)) / 1.5), -2.15f + (sin(float(timer ) + 1.0f)) / 10.0 , 0f, 1.0);


            spheres[6]= transform2 * kneeScale;

            transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                          0.0,1, 0.0,0.0,
                                          0.0, 0.0, 1, 0.0,
                                          (0.5f + (sin(float(timer ))) / 1.5), -2.9f , 0f, 1.0);


             spheres[7]= transform2 * kneeScale;

          mat4x4 abdomenScale = mat4x4(1, 0.0, 0.0, 0.0,
                                   0.0, 1, 0.0, 0.0,
                                   0.0, 0.0, 1, 0.0,
                                   0.0, 0.0, 0.0, 1.0);
          transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                        0.0,1, 0.0,0.0,
                                        0.0, 0.0, 1, 0.0,
                                        (1.8f), -0.75f, 0f, 1.0);


           spheres[8]= transform2 * abdomenScale;

           mat4x4 spineScale = mat4x4(0.75, 0.0, 0.0, 0.0,
                                    0.0, 0.75, 0.0, 0.0,
                                    0.0, 0.0, 0.75, 0.0,
                                    0.0, 0.0, 0.0, 1.0);


           transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                         0.0, 1.0, 0.0,0.0,
                                         0.0, 0.0, 1.0, 0.0,
                                         (- 0.7f), -0.2f, 0.0f, 1.0);


            spheres[9]= transform2 * spineScale;

            mat4x4 earScale = mat4x4(0.25, 0.0, 0.0, 0.0,
                                     0.0, 0.25, 0.0, 0.0,
                                     0.0, 0.0, 0.25, 0.0,
                                     0.0, 0.0, 0.0, 1.0);


            transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                          0.0, 1.0, 0.0,0.0,
                                          0.0, 0.0, 1.0, 0.0,
                                           (-1.7f), 0.4f, 0.0f, 1.0);


             spheres[10]= transform2 * earScale;

             mat4x4 backFootScale = mat4x4(0.5, 0.0, 0.0, 0.0,
                                      0.0, 0.25, 0.0, 0.0,
                                      0.0, 0.0, 0.25, 0.0,
                                      0.0, 0.0, 0.0, 1.0);


             transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                           0.0, 1.0, 0.0,0.0,
                                           0.0, 0.0, 1.0, 0.0,
                                            (3.0f + (sin(float(timer ) + 1.0f)) / 0.9), -3.1f + (sin(float(timer ) + 1.0f)) / 10.0 , 0.0f, 1.0);


              spheres[11]= transform2 * backFootScale;

              mat4x4 footScale = mat4x4(0.5, 0.0, 0.0, 0.0,
                                       0.0, 0.25, 0.0, 0.0,
                                       0.0, 0.0, 0.25, 0.0,
                                       0.0, 0.0, 0.0, 1.0);


              transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                            0.0, 1.0, 0.0,0.0,
                                            0.0, 0.0, 1.0, 0.0,
                                             (0.5f + (sin(float(timer ))) / 1.0f), -3.3f, 0.0f, 1.0);


               spheres[12]= transform2 * footScale;


               mat4x4 eyeScale = mat4x4(0.15, 0.0, 0.0, 0.0,
                                        0.0, 0.15, 0.0, 0.0,
                                        0.0, 0.0, 0.15, 0.0,
                                        0.0, 0.0, 0.0, 1.0);


               transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                             0.0, 1.0, 0.0,0.0,
                                             0.0, 0.0, 1.0, 0.0,
                                              (-1.9f), -0.2f, 0.0f, 1.0);


                spheres[13]= transform2 * eyeScale;

                mat4x4 muzzle = mat4x4(0.65, 0.0, 0.0, 0.0,
                                         0.0, 0.65, 0.0, 0.0,
                                         0.0, 0.0, 0.65, 0.0,
                                         0.0, 0.0, 0.0, 1.0);


                transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                              0.0, 1.0, 0.0,0.0,
                                              0.0, 0.0, 1.0, 0.0,
                                               (-1.9f), -0.75f, 0.0f, 1.0);


                 spheres[14]= transform2 * muzzle;

                 mat4x4 tail = mat4x4(0.15, 0.0, 0.0, 0.0,
                                          0.0, 0.15, 0.0, 0.0,
                                          0.0, 0.0, 0.15, 0.0,
                                          0.0, 0.0, 0.0, 1.0);


                 transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                               0.0, 1.0, 0.0,0.0,
                                               0.0, 0.0, 1.0, 0.0,
                                                (3.0f),0.05f, 0.0f, 1.0);


                  spheres[15]= transform2 * tail;

                  transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                                0.0, 1.0, 0.0,0.0,
                                                0.0, 0.0, 1.0, 0.0,
                                                 (3.15f), 0.1f, 0.0f, 1.0);


                   spheres[16]= transform2 * tail;

                   transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                                 0.0, 1.0, 0.0,0.0,
                                                 0.0, 0.0, 1.0, 0.0,
                                                  (3.25f), 0.2f, 0.0f, 1.0);


                    spheres[17]= transform2 * tail;

               for(int i = 0; i < SPHERENUM; i ++){
                   transform2 = mat4x4(1, 0.0 , 0.0,0.0,
                                                 0.0, 1.0, 0.0,0.0,
                                                 0.0, 0.0, 1.0, 0.0,
                                                  (-1.0f), sin( float(timer))/10.0, -10.0f, 1.0);

                   spheres[i] = spineScale*spheres[i];
                   spheres[i] = transform2*spheres[i];
               }
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
        float currT = raySphereIntersect((inverse(modelMat) * vec4(r0,1.0)).xyz, normalize((inverse(modelMat) * vec4(rd,0.0))).xyz, 0.5);

        if(currT != -1 && currT < bestT && (fromID != i)  ){
            bestT = currT;
            info.id = i;
            info.t = currT;
            info.modelMat = modelMat;
        }
    }
    return info;
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
        float currT = raySphereIntersect((inverse(currSphere.modelMat) * vec4(r0,1.0)).xyz, normalize((inverse(currSphere.modelMat) * vec4(rd,0.0)).xyz), 0.5);

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
    float ratioX = float(gl_FragCoord.x + 0.5)/float(693); // +0.5
    float ratioY = float(gl_FragCoord.y + 0.5)/float(566); //canvas XY
    float scaleX = ratioX * 2.0f - 1.0f;
    float scaleY = 1.0f - ratioY * 2.0f;
    vec4 screen =  vec4(scaleX, scaleY, -1.0f, 1.0f);
    return cam * screen;
}

mat4x4[SPHERENUM] makeSpheresForBumpmapDemo(){
    mat4x4 spheres[SPHERENUM];
    for(int i = 0; i < SPHERENUM; i++){
        spheres[i] = mat4x4(0.5, 0, 0, 0,
                            0, 0.5, 0, 0,
                            0, 0, 0.5, 0,
                            -2 + float(i)/2, -2 + float(i)/2 + i * sin(float(timer))/100.0, float(i) / 5.0, 1);
    }
    return spheres;
}

void main()
{
    fragColor = vec4(0.5,0.5,0.5, 1);
    float att = 1.0;

    mat4x4 spheres[SPHERENUM];
    if(bumpMapDemo == 0){
        spheres = makeSpheres();
    } else {
        spheres = makeSpheresForBumpmapDemo();
    }
    float sphereRadius = 1.0;
    vec3 rayStart = eye.xyz;

    rayDir = normalize(worldSpace.xyz - eye.xyz);

    // light location

    vec3 light = vec3(0  + 10 * cos(timer/15.0), 0 + sin(timer), -5 + cos(timer));
    if(bumpMapDemo == 1){
        light = vec3(10 * cos(timer/15.0), 0 + sin(timer), 5);
    }

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

    int numberOfBounces = 1;
    if(bumpMapDemo == 1){
        numberOfBounces = 2;
    }
    for(int bounce = 0; bounce < numberOfBounces; bounce++){
        // generate primary ray

        vec3 lightColor = lightColorConstant;
        IntersectInfo info = findCollision(spheres, SPHERENUM, rayStart, rayDir, bounceID);

        float res = info.t;
        bounceID = info.id;
        if(res != -1){
            intersected = true;
            if(bumpMapDemo == 0){
                calcColor.b = 1.0;
            }
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
            if(bounceID == 13){
                calcColor.b = 0.0;
                objectDiffuse = vec3(0.0,1.0,0.0); // the ground is green
                objectSpecular = vec3(0.0,0.0,0.0);

            } else {
                objectDiffuse = vec3(1.0,0.0,0.0);
                objectSpecular = vec3(1.0,1.0,1.0);

            }
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

//            calcColor = calcColor + vec3(0.0f,0.0f,0.0f);
//            calcColor = vec3(1,0,1);
        }

    }
    fragColor = vec4(calcColor, 1.0f);
    if (!intersected) {
        rayDir_data = texture(skybox, rayDir);
//        rayDir_data = vec4(rayDir, 0.0);
        fragColor = vec4(fragColor.b, fragColor.g, fragColor.r, 1.0);
    }
}
