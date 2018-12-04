//#version 330 core

//layout(location = 0) in vec2 in_position;
//layout(location = 5) in vec2 in_texCoord;

//out vec2 texCoord;

//void main() {
//    texCoord = in_texCoord;
//    gl_Position = vec4(in_position, 0.0, 1.0);
//}

#version 400 core

layout(location = 0) in vec3 position;
layout(location = 5) in vec2 inUV;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;
uniform int searchWidth;

out vec2 uv;

void main() {
    uv = inUV;
	vec4 position_cameraSpace = v * m * vec4(position, 1.0);
    gl_Position = p * position_cameraSpace;
}
