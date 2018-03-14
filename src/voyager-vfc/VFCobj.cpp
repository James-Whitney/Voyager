#include "include/VFCobj.h"

VFCobj::VFCobj(std::vector< std::shared_ptr<Component> > *objs) {
   std::shared_ptr<Renderable> curObj;
   std::shared_ptr<Shape> curShape;
   std::vector< std::shared_ptr<VFCbox> > boxes;
   for (int i = 0; i < objs->size(); i++) {
      curObj = static_pointer_cast<Renderable>(objs->at(i));
      if (curObj->getCullStatus()) {
         vec3 center = bulletToGlm(curObj->getEntity()->getTransform()->getOrigin());
         vec3 min = getMeshMin(curObj->getMesh())*bulletToGlm(*curObj->getEntity()->getScale())*2.0f;
         vec3 max = getMeshMax(curObj->getMesh())*bulletToGlm(*curObj->getEntity()->getScale())*2.0f;
         float radius = sqrt(pow((max.x-min.x),2)+
                             pow((max.y-min.y),2)+
                             pow((max.z-min.z),2))/2.0f;
         std::vector<int> idx;
         idx.push_back(i);
         boxes.push_back(std::make_shared<VFCbox>(center, min, max, radius, &idx));
      } else {
         this->dynamic.push_back(i);
      }
   }

   this->tree = std::make_shared<VFCoct>(boxes);
}

void VFCobj::ExtractVFPlanes(mat4 P, mat4 V) {
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

float VFCobj::DistToPlane(float A, float B, float C, float D, vec3 point) {
   return (A*point.x) + (B*point.y) + (C*point.z) + D;
}

std::vector<int> VFCobj::ViewFrustCull() {
   vec3 center = this->tree->box->center;
   float radius = this->tree->box->radius;
   std::vector<int> cullIdxs;

   CullTree(this->tree, &cullIdxs);

   return getIdxToRender(this->tree->box->idxs, cullIdxs);
}

void VFCobj::CullTree(std::shared_ptr<VFCoct> curTree, std::vector<int> *rmvIdx) {
   bool keep = true;
   float dist;
   for (int i = 0; i < 6; i++) {
      dist = DistToPlane(this->planes[i].x, this->planes[i].y, this->planes[i].z, this->planes[i].w, curTree->box->center);
      if (dist < -curTree->box->radius) {
         keep = false;
         break;
      }
   }
   if (keep) {
      for (auto &child : curTree->children) {
         if (NULL != child) { CullTree(child, rmvIdx); }
      }
   } else {
      for (auto &i : curTree->box->idxs) { rmvIdx->push_back(i); }
   }
}

vec3 VFCobj::getMeshMin(std::vector<std::shared_ptr<Shape>> shapes) {
   vec3 min = vec3(std::numeric_limits<float>::infinity());
   for (auto &shape : shapes) {
      if (isinf(min.x) || shape->min.x < min.x) { min.x = shape->min.x; }
      if (isinf(min.y) || shape->min.y < min.y) { min.y = shape->min.y; }
      if (isinf(min.z) || shape->min.z < min.z) { min.z = shape->min.z; }
   }
   return min;
}

vec3 VFCobj::getMeshMax(std::vector<std::shared_ptr<Shape>> shapes) {
   vec3 max = vec3(-1 * std::numeric_limits<float>::infinity());
   for (auto &shape : shapes) {
      if (isinf(max.x) || shape->max.x > max.x) { max.x = shape->max.x; }
      if (isinf(max.y) || shape->max.y > max.y) { max.y = shape->max.y; }
      if (isinf(max.z) || shape->max.z > max.z) { max.z = shape->max.z; }
   }
   return max;
}

//http://www.cplusplus.com/forum/general/22332/#msg116987
std::vector<int> VFCobj::getIdxToRender(std::vector<int> &model, std::vector<int> &pattern) {
   std::set<int> s_model(model.begin(), model.end());
   std::set<int> s_pattern(pattern.begin(), pattern.end());
   std::vector<int> result;

   std::set_difference(s_model.begin(), s_model.end(), s_pattern.begin(), s_pattern.end(),
      std::back_inserter(result));

   result.insert(result.end(), this->dynamic.begin(), this->dynamic.end());

   return result;
}
