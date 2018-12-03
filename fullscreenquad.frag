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

uniform sampler2D tex;

out vec4 fragColor;

void main(){
    fragColor = vec4(1.0);

    // TODO [Task 9] Calculate the texelSize
    vec2 texelSize = 1.0/ textureSize(tex,0).xy;

    const int supportWidth = 20;

    fragColor = vec4(0.0);
	vec4 currColor = texture(tex, uv);
    float weights = 1.0;
	int colored = 0;

    for (int i = -supportWidth; i <= supportWidth; i++) {
        float weight = (supportWidth + 1) - abs(i);
        // TODO [Task 9] Add weight * sampleColor to fragColor, where
        //               sampleColor = tex sampled at uv + the offset
        //               in the x direction (you are moving over by "i" texels)

        //uv + (texelSize * i)
        vec2 texSample = uv + texelSize * float(i);

        vec2 wh = vec2(texSample.x, uv.y);
        vec4 sampleColor = texture(tex, wh);

	if(sampleColor.r != 0.0 && currColor.r == 0.0){// && int(gl_FragCoord.y) % 10 == 0){
		//fragColor = vec4(1.0,1.0,1.0,1.0);
               colored = 1;
	} else {
		weights = weight;
	}
        
    }
//fragColor += vec4(1.0,0.0,0.0,1.0) / weights;
    //fragColor /= weights;
//if(colored == 0){
   fragColor = texture(tex, uv);
//}
}
