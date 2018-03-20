#version 330 core

layout(location = 0) in vec4 vertPos;

uniform mat4 P;
uniform mat4 V;

out vec3 skyboxTexCoord;

void main() {
   gl_Position = P * mat4(mat3(V)) * vertPos;
   skyboxTexCoord = vertPos.xyz;
}
