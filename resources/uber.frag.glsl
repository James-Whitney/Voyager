#version 330 core

in vec3 fragNor;
in vec3 WPos;

layout(location = 0) out vec4 color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 MatAmb;
uniform vec3 MatDif;

uniform uint uberMode;

// Used for Cook Torrance
uniform float opacity;
uniform float roughnessValue;
uniform float F0;
uniform float K;

void main() {

   vec3 normal;

   switch (uberMode) {

   /* --=[ Cook Torrance ]=-------------------------------------------------- */
   case 1:
      // interpolating normals will change the length, renormalize
      normal = normalize(fragNor);
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
      color = vec4(finalValue, opacity);
      break;

   /* --=[ Blinn-Phong ]=---------------------------------------------------- */
   case 2:
      color = vec4(1, 0, 0, 1); // NOT IMPLEMENTED
      break;

   /* --=[ Default Shading ]=------------------------------------------------ */
   default:
      vec3 Dcolor, Scolor;
      normal = normalize(fragNor);
      Dcolor = MatDif * lightColor * max(dot(normalize(lightPos), normal), 0) + MatAmb;
      color = vec4(Dcolor, 1.0);


   }
}
