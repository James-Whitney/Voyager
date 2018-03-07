#version 330 core

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform mat4 shadowP;
uniform mat4 shadowV;
uniform int shadowMode;

out vec3 fragNor;
out vec3 wFragNor;
out vec3 wFragPos;
out vec3 WPos;
out vec4 shadowCoord;
out vec4 view;

void main() {

   if (shadowMode > 0) {
       gl_Position = shadowP * shadowV * M * vertPos;
   } else {
       gl_Position = P * V * M * vertPos;
   }

   fragNor = (V * M * vec4(vertNor, 0.0)).xyz;
   wFragNor = vertNor;
   wFragPos = vertPos.xyz;
   WPos = vec3(V * M * vertPos);

   shadowCoord = shadowP * shadowV * M * vertPos;

   view = V * M * vertPos;
}
