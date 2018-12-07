//#version 330 core

//in vec2 texCoord;

//uniform sampler2D tex;

//out vec4 fragColor;

//void main() {
//   fragColor = vec4(1.0,1.0,1.0,1.0);

//texture(tex, texCoord);
//}

#version 400 core

in vec2 uv;

uniform int searchWidth;
uniform sampler2D tex;
uniform int finalFill = 0;

out vec4 fragColor;
// Professor Ralph Lingonberry the 3rd
void main(){
    fragColor = vec4(1.0);

    // TODO [Task 9] Calculate the texelSize
    vec2 texelSize = 1.0/ textureSize(tex,0).xy;

    int supportWidth = searchWidth;

    fragColor = vec4(0.0);
    vec4 currColor = texture(tex, uv);
    if((currColor.r != 0.0 && currColor.r != 1.0) || (currColor.b != 0.0 ) ){
        fragColor = currColor;
    } else {
        int colored = 0;

        for (int i = -supportWidth; i <= supportWidth; i++) {
            for(int j = -supportWidth; j <= supportWidth; j++){

                // TODO [Task 9] Add weight * sampleColor to fragColor, where
                //               sampleColor = tex sampled at uv + the offset
                //               in the x direction (you are moving over by "i" texels)

                //uv + (texelSize * i)
                vec2 texSampleX = uv + texelSize * float(i);
                vec2 texSampleY = uv + texelSize * float(j);
                vec2 wh = vec2(texSampleX.x, texSampleY.y);
                vec4 sampleColor = texture(tex, wh);

                if((sampleColor.r != 0.0) || (sampleColor.b != 0.0) ){
                    colored += 1;
                }
            }
        }
        int area = (supportWidth*2+1) * (supportWidth*2+1);
        float threshHold = floor(area/3);

        if(colored >  int(threshHold)){
            fragColor = vec4(1.0, 1.0, 1.0, 1.0);
        } else {
            fragColor = currColor;
        }
   }
   if(finalFill == 1){
       if(fragColor.r == 0.0 && fragColor.g == 0.0 && fragColor.b == 0.0){

       } else {
           if(fragColor.r == 0.0){
               fragColor.r += 0.1;
           }
//           float random = fract(sin(dot(uv.xy, vec2(12.9898,78.233))) * 43758.5453);
//           if(random < 0.25){
//               fragColor = vec4(0.0,0.0,0.0,1.0);
//           }
           fragColor = vec4(fragColor.r, fragColor.r, fragColor.r, 1.0);

       }

   }


}
