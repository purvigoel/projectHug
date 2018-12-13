#version 400 core

in vec2 uv;

uniform int searchWidth;
uniform sampler2D tex;
uniform sampler2D texture2;
uniform sampler2D rayDir_data;
uniform int finalFill = 0;

uniform int timer = 0;

uniform int justFur = 0;

out vec4 fragColor;

vec2 projection(vec2 point){
    float tanZX = atan(point.y + 0.5, point.x + 0.5);
    // if its negative, we'll just stick it in the positive quadrant
    if(tanZX < 0.0f){
        tanZX = atan(-point.y - 0.5, -point.x - 0.5) +3.14/2 ;
    }
    float fraction = (tanZX)/ (2* 3.14/2);

    // it ends up backwards if I dont subtract from 1
    return vec2(fraction  , point.y);
}

float getFurDensity(vec2 uv, float distanceFromCenter){
    float densityThreshold = 0.0;
  //  uv = vec2(0.2, 0.5);
    vec2 sphericalUV = projection(uv);
    vec2 texelSize = 1.0/ textureSize(tex,0).xy;

    uv.y += texelSize.y * sin(float(timer));
    uv.x += texelSize.x * cos(float(timer));

    vec4 randomDensity = texture(texture2, sphericalUV);//texture(tex, uv); //vec4(0.5,0.5,0.5,0.5);
    float random1 = fract(sin(dot(uv.xy, vec2(12.9898,78.233))) * 43758.5453);
    float random2 = fract(cos(dot(uv.xy, vec2(12.9898,78.233))) * 43758.5453);
    randomDensity.x += random1;
    randomDensity.y += random2;

    float density = clamp((randomDensity.x - densityThreshold)/(1.0 - densityThreshold), 0.0, 1.0);
    density = density * density * (3.0 - 2.0 * density);
    float radius = distanceFromCenter;
    float maxDepth = 0.2;
    float t = (radius - (1.0 - maxDepth)) / maxDepth;
    float strandLength = randomDensity.y;
    strandLength = clamp((t - strandLength - 0.2)/(strandLength - (strandLength - 0.2)), 0.0, 1.0);
    strandLength = strandLength * strandLength * (3.0 - 2.0 * strandLength);
    density *= strandLength;
    return density;
}

vec4 sampleFurColor(float distanceFromCenter, vec2 uv, vec4 baseColor){
    float radius = distanceFromCenter; //length(uv);
    float maxDepth = 0.2;
    float t = (radius - (1.0 - maxDepth)) / maxDepth;
    t = clamp(t, 0.0, 1.0);
    float i = t * 0.5 + 0.5;
    return baseColor * (texture(tex, uv) * 1.5 )* i ;
}

vec4 furColor(vec2 uv, vec4 baseColor){
    float maxDepth = 0.5;
    int maxFurLayers = 60;
    vec4 finalColor = vec4(0.0);
    float totalDensity = 0.0;
    float densityThreshold = 0.95;
    vec2 texelSize = 1.0/ textureSize(tex,0).xy;
    vec2 sampleAt = uv;
    for(int i = 0; i < maxFurLayers; i++){
       vec4 color = vec4(0.0);
       float distanceFromCenter = (maxFurLayers - i) * (1.0 / float(maxFurLayers));
       float density = getFurDensity(sampleAt, float(i));
       if(density > 0.3 ){
           color = sampleFurColor(distanceFromCenter, sampleAt, baseColor);
           color *= density;
           finalColor = finalColor + color * (1.0 - density);
           finalColor.a = 1.0;
           totalDensity = totalDensity + density * (1.0 - density);
           if(totalDensity > densityThreshold){
               break;
           }
       }

       vec2 moveSampleBy = texelSize;
       sampleAt = sampleAt + moveSampleBy;

       if(finalFill == 0){
           finalColor = vec4(float(i)/float(maxFurLayers),float(i)/float(maxFurLayers),float(i)/float(maxFurLayers),1.0 );
       }
    }
    if(finalColor.r == 0.0){
        finalColor.w = 0.1;
    }
    return finalColor;
}

void main(){
//    fragColor = vec4(0.0);

    vec2 size = textureSize(tex,0).xy;
    vec2 texelSize = 1.0 / textureSize(tex,0).xy;
    vec4 currColor = texture(tex, uv);
    if((currColor.g == 0.0 && currColor.b == 0.0)|| (currColor.g != 0.0 && currColor.r == 0.0 && currColor.b==0.0)){

        if(currColor.g != 0.0 && currColor.r == 0.0 && currColor.b==0.0) {
            fragColor = vec4(0.0,0.0,0.0,1.0);
            if((currColor.g != 0.0 && currColor.r == 0.0 && currColor.b==0.0)){
                fragColor = vec4(1.0,1.0,1.0,1.0);
            }
        } else {
            fragColor = texture(rayDir_data, uv);
            fragColor = vec4(fragColor.b , fragColor.g, fragColor.r, 1.0);
        }

    } else {
        fragColor = currColor;
        float random1 = fract(sin(dot(uv.xy, vec2(12.9898,78.233))) * 43758.5453);
        float search = 4.0;
        float indexX = random1 * search - (search - 1);
        float random2 = fract(sin(dot(uv.xy, vec2(12.9898,78.233))) * 43758.5453);
        float indexY = random2 * search - (search - 1);
        vec4 newColor = texture(tex, vec2(indexX * texelSize.x+ uv.x, indexY* texelSize.y + uv.y));


        //fragColor = vec4(newColor.r * 0.5 + (currColor.r * 0.5), newColor.r * 0.5 + (currColor.r * 0.5), newColor.r * 0.5 + (currColor.r * 0.5), 1.0);
        vec4 texColor = texture(texture2, uv.xy * 1.5);        

        if(finalFill == 0){
            texColor = vec4(0.0,0.0,0.0,1.0);
        }
        texColor = vec4(texColor.b, texColor.g, texColor.r, 1.0);
        fragColor = newColor;

        vec4 baseColor =  texColor * (fragColor.r * 1.5);
        vec4 finalColor =  furColor(uv, baseColor);

        if(finalColor.w != 0.1){
            fragColor = finalColor * 1.5;
        } else {
            fragColor = texture(rayDir_data, uv);
            fragColor = vec4(fragColor.b , fragColor.g, fragColor.r, 1.0);
        }

    }
}
