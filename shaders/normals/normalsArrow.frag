#version 400 core

in vec2 uv;

uniform int searchWidth;
uniform sampler2D tex;
uniform sampler2D texture2;
uniform int finalFill = 0;


out vec4 fragColor;
// Professor Ralph Lingonberry the 3rd
void main(){
    fragColor = vec4(1.0);
    vec2 size = textureSize(tex,0).xy;
    vec2 texelSize = 1.0/ textureSize(tex,0).xy;
    vec4 currColor = texture(tex, uv);
    if((currColor.g == 0.0 && currColor.b == 0.0)|| (currColor.g != 0.0 && currColor.r == 0.0 && currColor.b==0.0)){
        //fragColor = currColor;
        if(gl_FragCoord.y < size.y/3){
            fragColor = vec4(0.0,1.0,0.0,1.0);
        } else {
            fragColor = vec4(0.0,0.0,1.0,1.0);
        }
    } else {
//    if(currColor.g != 0.0){
        fragColor = currColor;
        float random1 = fract(sin(dot(uv.xy, vec2(12.9898,78.233))) * 43758.5453);
        float search = 4.0;
        float indexX = random1 * search - (search - 1);
        float random2 = fract(sin(dot(uv.xy, vec2(12.9898,78.233))) * 43758.5453);
        float indexY = random2 * search - (search - 1);
        vec4 newColor = texture(tex, vec2(indexX * texelSize.x+ uv.x, indexY* texelSize.y + uv.y));


        fragColor = vec4(newColor.r * 0.5 + (currColor.r * 0.5), newColor.r * 0.5 + (currColor.r * 0.5), newColor.r * 0.5 + (currColor.r * 0.5), 1.0);
        vec4 texColor = texture(texture2, uv.xy);
        texColor = vec4(texColor.b, texColor.g, texColor.r, 1.0);
        //fragColor = vec4(fragColor.r * 0.4 + texColor.r * 0.6, fragColor.g * 0.4 + texColor.g * 0.6, fragColor.b * 0.4 + texColor.b * 0.6, 1.0);
        fragColor = texColor * (fragColor.r * 1.1);

    }


}
