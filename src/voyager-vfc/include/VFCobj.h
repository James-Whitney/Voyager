#pragma once
#ifndef VOYAGER_VFC_OBJ_H
#define VOYAGER_VFC_OBJ_H

#include <glad/glad.h>
#include <voyager-utils/include/BulletToGlm.h>
#include <bullet/src/btBulletDynamicsCommon.h>
#include <voyager-hud/include/Hud.h>
#include <voyager-utils/include/Shape.h>
#include <voyager-render/include/Renderable.h>

#include <math.h>
#include <set>

#include "VFCbox.h"
#include "VFCoct.h"

using namespace glm;

class VFCobj {
public:
   vec4 Left, Right, Bottom, Top, Near, Far;
   vec4 planes[6];

   std::shared_ptr<VFCoct> tree;
   std::vector<int> dynamic;

   VFCobj(std::vector< std::shared_ptr<Component> > *objs);
   void ExtractVFPlanes(mat4 P, mat4 V);
   float DistToPlane(float A, float B, float C, float D, vec3 point);
   std::vector<int> ViewFrustCull();
   void CullTree(std::shared_ptr<VFCoct> curTree, std::vector<int> *rmvIdx);
   vec3 getMeshMin(std::vector<std::shared_ptr<Shape>> shapes);
   vec3 getMeshMax(std::vector<std::shared_ptr<Shape>> shapes);
   //http://www.cplusplus.com/forum/general/22332/#msg116987
   std::vector<int> getIdxToRender(std::vector<int> &model, std::vector<int> &pattern);
};

#endif
