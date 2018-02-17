#include "Renderable.h"
#include <voyager-utils/include/Shape.h>
#include <glm/glm/gtc/type_ptr.hpp>

void ExtractVFPlanes(mat4 *planes, mat4 P, mat4 V)
{
   vec4 Left, Right, Bottom, Top, Near, Far;

   /* composite matrix */
   mat4 comp = P * V;
   vec3 n; //use to pull out normal
   float l; //length of normal for plane normalization

   Left.x = comp[0][3] + comp[0][0];
   Left.y = comp[1][3] + comp[1][0];
   Left.z = comp[2][3] + comp[2][0];
   Left.w = comp[3][3] + comp[3][0];
   Left /= length(vec3(Left.x, Left.y, Left.z));
   planes[0] = Left;

   Right.x = comp[0][3] - comp[0][0];
   Right.y = comp[1][3] - comp[1][0];
   Right.z = comp[2][3] - comp[2][0];
   Right.w = comp[3][3] - comp[3][0];
   Right /= length(vec3(Right.x, Right.y, Right.z));
   planes[1] = Right;

   Bottom.x =  comp[0][3] + comp[0][1];
   Bottom.y =  comp[1][3] + comp[1][1];
   Bottom.z =  comp[2][3] + comp[2][1];
   Bottom.w =  comp[3][3] + comp[3][1];
   Bottom /= length(vec3(Bottom.x, Bottom.y, Bottom.z));
   planes[2] = Bottom;

   Top.x =  comp[0][3] - comp[0][1];
   Top.y =  comp[1][3] - comp[1][1];
   Top.z =  comp[2][3] - comp[2][1];
   Top.w =  comp[3][3] - comp[3][1];
   Top /= length(vec3(Top.x, Top.y, Top.z));
   planes[3] = Top;

   Near.x =  comp[0][3] + comp[0][2];
   Near.y =  comp[1][3] + comp[1][2];
   Near.z =  comp[2][3] + comp[2][2];
   Near.w =  comp[3][3] + comp[3][2];
   Near /= length(vec3(Near.x, Near.y, Near.z));
   planes[4] = Near;

   Far.x =  comp[0][3] - comp[0][2];
   Far.y =  comp[1][3] - comp[1][2];
   Far.z =  comp[2][3] - comp[2][2];
   Far.w =  comp[3][3] - comp[3][2];
   Far /= length(vec3(Far.x, Far.y, Far.z));
   planes[5] = Far;
}


/* helper function to compute distance to the plane */
float DistToPlane(float A, float B, float C, float D, vec3 point)
{
   return A*point.x + B*point.y + C*point.z + D;
}

//returns 1 to CULL
int ViewFrustCull(mat4 planes, vec3 center, float radius)
{
   float dist;

   for (int i = 0; i < 6; i++)
   {
      dist = DistToPlane(planes[i].x, planes[i].y, planes[i].z, planes[i].w, center);
      if (dist < radius)
      {
         return 1;
      }
   }
   return 0;
}

vec3 getCenterRenderable(std::shared_ptr<Renderable> renderable) {
   glm::vec3 min = renderable->getShape()->min;
   glm::vec3 max = renderable->getShape()->max;
   return vec3((min.x+max.x)/2.0f,(min.y+max.y)/2.0f,(min.z+max.z)/2.0f)
}
