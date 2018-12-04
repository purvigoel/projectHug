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
    if(currColor.r == 0.0 && currColor.g == 0.0 && currColor.b == 0.0){
       fragColor = currColor;
    } else {
        float weights = 0.0;
        for (int i = -supportWidth; i <= supportWidth; i++) {
            float weight = (supportWidth + 1) - abs(i);
            // TODO [Task 9] Add weight * sampleColor to fragColor, where
            //               sampleColor = tex sampled at uv + the offset
            //               in the x direction (you are moving over by "i" texels)

            //uv + (texelSize * i)
            vec2 texSample = uv + texelSize * float(i);

            vec2 wh = vec2(texSample.x, uv.y);
            vec4 sampleColor = texture(tex, wh);
            if(sampleColor.r == 0.0 && sampleColor.g == 0.0 && sampleColor.b == 0.0){
                continue;
            }
            weights += weight;

            fragColor += sampleColor * weight;
        }
        fragColor /= weights;
    }

}
