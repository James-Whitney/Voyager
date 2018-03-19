#version 330 core

in vec3 skyboxTexCoord;

layout(location = 0) out vec4 color;

uniform samplerCube cubeMap;

uniform vec3 fogColor;
uniform float fogHeight;

void main() {
   color = texture(cubeMap, skyboxTexCoord);

   if (skyboxTexCoord.y < fogHeight) {
      float mixFactor = skyboxTexCoord.y / fogHeight;
      mixFactor = clamp(mixFactor, 0.0, 1.0);
      color = mix(vec4(fogColor, 1.0), color, mixFactor);
   }
}
