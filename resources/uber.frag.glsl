#version 330 core

in vec3 fragNor;
in vec3 wFragNor;
in vec3 wFragPos;
in vec3 WPos;
in vec4 shadowCoord;

layout(location = 0) out vec4 color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 MatAmb;
uniform vec3 MatDif;

uniform uint uberMode;
uniform int shadowMode;
uniform sampler2D depthTexture;

// Used for Cook Torrance
uniform float opacity;
uniform float roughnessValue;
uniform float F0;
uniform float K;

uniform sampler2D terrainTexture;

vec4 cookTorrance() {
   // interpolating normals will change the length, renormalize
   vec3 normal = normalize(fragNor);
   vec3 lightDirection = lightPos - WPos;
   float lightDistance = length(lightDirection);

   // do the lighting calculation for each fragment
   float NdotL = max(dot(normal, lightDirection), 0.0);

   float specular = 0.0;
   if (NdotL > 0.0) {
      vec3 eyeDir = normalize(-WPos);

      // calculate intermediary values
      vec3 halfVector = normalize(lightDirection + eyeDir);
      float NdotH = max(dot(normal, halfVector), 0.0);
      float NdotV = max(dot(normal, eyeDir), 0.0);
      float VdotH = max(dot(eyeDir, halfVector), 0.0);
      float mSquared = roughnessValue * roughnessValue;

      // geometric attenuation
      float NH2 = 2.0 * NdotH;
      float g1 = (NH2 * NdotV) / VdotH;
      float g2 = (NH2 * NdotL) / VdotH;
      float geoAtt = min(1.0, min(g1, g2));

      // roughness beckmann distribution function
      float r1 = 1.0 / (4.0 * mSquared * pow(NdotH, 4.0));
      float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
      float roughness = r1 * exp(r2);

      // fresnel Schlick approximation
      float fresnel = pow(1.0 - VdotH, 5.0);
      fresnel *= (1.0 - F0);
      fresnel += F0;

      specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.1415926);
   }

   vec3 finalValue = MatAmb + (lightColor / lightDistance) * NdotL * (K + specular * (1.0 - K));
   return vec4(finalValue, opacity);
}

void main() {

   vec3 normal;

   switch (uberMode) {

   /* --=[ Cook Torrance ]=-------------------------------------------------- */
   case 1:
      color = cookTorrance();
      break;

   /* --=[ Blinn-Phong ]=---------------------------------------------------- */
   case 2:
      color = vec4(1, 0, 0, 1); // NOT IMPLEMENTED
      break;

   /* --=[ Normal Shading ]=------------------------------------------------- */
   case 3:
      normal = normalize(fragNor);
      vec3 ncolor = 0.5 * normal + 0.5;
      color = vec4(ncolor, 1.0);
      break;

   /* --=[ Terrain Cook Torrance ]=------------------------------------------------- */
   case 4:
      // Tri-planar texture mapping
      vec3 wNorm = normalize(wFragNor);
      vec3 blending = normalize(max(abs(wNorm), 0.00001));
      float b = (blending.x + blending.y + blending.z);
      blending /= vec3(b, b, b);

      float scale = 0.1;
      vec4 xaxis = texture(terrainTexture, wFragPos.yz * scale);
      vec4 yaxis = texture(terrainTexture, wFragPos.xz * scale);
      vec4 zaxis = texture(terrainTexture, wFragPos.xy * scale);
      vec4 tex = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;

      color = tex * cookTorrance();
      break;

   /* --=[ Default Shading ]=------------------------------------------------ */
   default:
      vec3 Dcolor, Scolor;
      normal = normalize(fragNor);
      Dcolor = MatDif * lightColor * max(dot(normalize(lightPos), normal), 0) + MatAmb;
      color = vec4(Dcolor, 1.0);


   }

   if (shadowMode > 0) {
       vec3 shift = shadowCoord.xyz * 0.5 + vec3(0.5);
       color = vec4(shift.z, 0.0, 0.0, 1.0);
   } else {
        vec3 shift = shadowCoord.xyz * 0.5 + vec3(0.5);
        float depth = texture(depthTexture, shift.xy).r;

        float bias = 0.005;
        if (depth < (shift.z - bias)) {
           color = 0.5 * color;
        }
   }
}
