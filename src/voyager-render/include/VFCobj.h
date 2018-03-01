#pragma once
#ifndef VOYAGER_VFC_OBJ_H
#define VOYAGER_VFC_OBJ_H

#include <glad/glad.h>
#include <voyager-utils/include/BulletToGlm.h>
#include <bullet/src/btBulletDynamicsCommon.h>
#include <voyager-hud/include/Hud.h>

#include <math.h>

#include "VFCbox.h"
#include "VFCoct.h"

using namespace glm;

class VFCobj {
public:
   vec4 Left, Right, Bottom, Top, Near, Far;
   vec4 planes[6];

   std::shared_ptr<VFCoct> tree;

   VFCobj(std::vector< std::shared_ptr<Component> > *objs) {
      std::shared_ptr<Renderable> curObj;
      std::vector< std::shared_ptr<VFCbox> > boxes;
      for (int i = 0; i < objs->size(); ++i) {
         curObj = std::static_pointer_cast<Renderable>(objs->at(i));
         vec3 center = bulletToGlm(curObj->getEntity()->getTransform()->getOrigin());
         vec3 min = curObj->getShape()->min;
         vec3 max = curObj->getShape()->max;
         float radius = curObj->getShape()->radius;
         std::vector<int> idx;
         idx.push_back(i);
         boxes.push_back(std::make_shared<VFCbox>(center, min, max, radius, &idx));
      }
      this->tree = std::make_shared<VFCoct>(boxes);
   }

   void ExtractVFPlanes(mat4 P, mat4 V) {
      mat4 comp = P * V;

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

   float DistToPlane(float A, float B, float C, float D, vec3 point) {
      return (A*point.x) + (B*point.y) + (C*point.z) + D;
   }

   std::vector<int> ViewFrustCull() {
      vec3 center = this->tree->box->center;
      float radius = this->tree->box->radius;
      std::vector<int> idxs;

      CullTree(this->tree, &idxs);
      /*
      float dist;
      for (int i = 0; i < 6; i++) {
         dist = DistToPlane(this->planes[i].x, this->planes[i].y, this->planes[i].z, this->planes[i].w, center);
         if (dist < -radius) {
            return 1;
         }
      }
      return 0;
      */
      return idxs;
   }

   void CullTree(std::shared_ptr<VFCoct> curTree, std::vector<int> *rmvIdx) {
      bool checknext = true;
      float dist;
      for (int i = 0; i < 6; i++) {
         dist = DistToPlane(this->planes[i].x, this->planes[i].y, this->planes[i].z, this->planes[i].w, curTree->box->center);
         if (dist > curTree->box->radius) {
            for (auto &i : curTree->box->idxs) { rmvIdx->push_back(i); }
            checknext = false;
            break;
         }
      }
      if (checknext) {
         for (auto &child : curTree->children) {
            if (NULL != child) { CullTree(child, rmvIdx); }
         }
      }
   }


};

#endif
