#pragma once
#ifndef VOYAGER_VIEWFRUSTUMCULLING_H
#define VOYAGER_VIEWFRUSTUMCULLING_H

#include <glad/glad.h>
#include <voyager-utils/include/BulletToGlm.h>
#include <bullet/src/btBulletDynamicsCommon.h>

#include <math.h>

using namespace glm;

class VFC {
public:
   vec4 Left, Right, Bottom, Top, Near, Far;
   vec4 planes[6];

   VFC() { }

   void ExtractVFPlanes(mat4 P, mat4 V) {
      /* composite matrix */
      mat4 comp = P * V;
      //vec3 n; //use to pull out normal
      float l; //length of normal for plane normalization

      this->Left.x = comp[0][3] + comp[0][0];
      this->Left.y = comp[1][3] + comp[1][0];
      this->Left.z = comp[2][3] + comp[2][0];
      this->Left.w = comp[3][3] + comp[3][0];
      this->Left /= length(vec3(this->Left.x, this->Left.y, this->Left.z));
      this->planes[0] = this->Left;

      this->Right.x = comp[0][3] - comp[0][0];
      this->Right.y = comp[1][3] - comp[1][0];
      this->Right.z = comp[2][3] - comp[2][0];
      this->Right.w = comp[3][3] - comp[3][0];
      this->Right /= length(vec3(this->Right.x, this->Right.y, this->Right.z));
      this->planes[1] = this->Right;

      this->Bottom.x =  comp[0][3] + comp[0][1];
      this->Bottom.y =  comp[1][3] + comp[1][1];
      this->Bottom.z =  comp[2][3] + comp[2][1];
      this->Bottom.w =  comp[3][3] + comp[3][1];
      this->Bottom /= length(vec3(this->Bottom.x, this->Bottom.y, this->Bottom.z));
      this->planes[2] = this->Bottom;

      this->Top.x =  comp[0][3] - comp[0][1];
      this->Top.y =  comp[1][3] - comp[1][1];
      this->Top.z =  comp[2][3] - comp[2][1];
      this->Top.w =  comp[3][3] - comp[3][1];
      this->Top /= length(vec3(this->Top.x, this->Top.y, this->Top.z));
      this->planes[3] = this->Top;

      this->Near.x =  comp[0][3] + comp[0][2];
      this->Near.y =  comp[1][3] + comp[1][2];
      this->Near.z =  comp[2][3] + comp[2][2];
      this->Near.w =  comp[3][3] + comp[3][2];
      this->Near /= length(vec3(this->Near.x, this->Near.y, this->Near.z));
      this->planes[4] = this->Near;

      this->Far.x =  comp[0][3] - comp[0][2];
      this->Far.y =  comp[1][3] - comp[1][2];
      this->Far.z =  comp[2][3] - comp[2][2];
      this->Far.w =  comp[3][3] - comp[3][2];
      this->Far /= length(vec3(this->Far.x, this->Far.y, this->Far.z));
      this->planes[5] = this->Far;
   }


   /* helper function to compute distance to the plane */
   float DistToPlane(float A, float B, float C, float D, vec3 point) {
      return A*point.x + B*point.y + C*point.z + D;
   }

   int ViewFrustCull(std::shared_ptr<Renderable> renderable) {
      vec3 center = bulletToGlm(renderable->getEntity()->getTransform()->getOrigin());

      vec3 min = renderable->getShape()->min;
      vec3 max = renderable->getShape()->max;
      float radius = 2.0f;
      float dist;

      for (int i = 0; i < 6; i++) {
         dist = DistToPlane(this->planes[i].x, this->planes[i].y, this->planes[i].z, this->planes[i].w, center);
         if (dist < radius) {
            return 1;
         }
      }
      return 0;
   }
};

#endif
