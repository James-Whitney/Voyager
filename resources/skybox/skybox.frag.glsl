#version 330 core

in vec3 skyboxTexCoord;

layout(location = 0) out vec4 color;

uniform samplerCube cubeMap;

void main() {
   // color = vec4(skyboxTexCoord, 1.0);
   color = texture(cubeMap, skyboxTexCoord);
}
