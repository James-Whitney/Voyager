#pragma once
#ifndef VOYAGER_VIEWFRUSTUMCULLING_H
#define VOYAGER_VIEWFRUSTUMCULLING_H

#include <glad/glad.h>
#include <voyager-utils/include/BulletToGlm.h>
#include <bullet/src/btBulletDynamicsCommon.h>
#include <voyager-hud/include/Hud.h>

#include <math.h>

using namespace glm;

class VFC {
public:
   vec4 Left, Right, Bottom, Top, Near, Far;
   vec4 planes[6];

   

   VFC() { }

   void ExtractVFPlanes(mat4 P, mat4 V);
   /* helper function to compute distance to the plane */
   float DistToPlane(float A, float B, float C, float D, vec3 point);
   int ViewFrustCull(std::shared_ptr<Renderable> renderable, std::shared_ptr<Hud> hud, int i);
};

#endif
