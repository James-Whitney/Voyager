#pragma once
#ifndef VOYAGER_VFC_OCT_H
#define VOYAGER_VFC_OCT_H

#include <glad/glad.h>
#include <math.h>
#include <limits>

#include "VFCbox.h"

using namespace glm;

class VFCoct {
public:
   std::shared_ptr<VFCoct> children[8];
   std::vector< std::shared_ptr<VFCbox> > objs;
   VFCbox *box;

   VFCoct(std::vector< std::shared_ptr<VFCbox> > box) {
      for (auto &b : box) { this->objs.push_back(b); }
      generateBox();
      generateChildren();
   }

   void generateBox() {
      std::vector<int> idxs;
      vec3 min = vec3(std::numeric_limits<float>::infinity());
      vec3 max = vec3(-1 * std::numeric_limits<float>::infinity());
      for (auto &temp : this->objs) {
         for (auto &i : temp->idxs) { idxs.push_back(i); }
         replaceMinest(&min, &temp->min);
         replaceMaxest(&min, &temp->max);
      }
      vec3 center = vec3((max.x+min.x)/2.0f,(max.y+min.y)/2.0f,(max.y+min.y)/2.0f);
      float radius = sqrt(pow((max.x-min.x),2)+pow((max.y-min.y),2)+pow((max.z-min.z),2));
      this->box = new VFCbox(center, min, max, radius, &idxs);
   }

   void replaceMinest(vec3 *a, vec3 *b) {
      if (b->x < a->x) { b->x = a->x; }
      if (b->y < a->y) { b->y = a->y; }
      if (b->z < a->z) { b->z = a->z; }
   }

   void replaceMaxest(vec3 *a, vec3 *b) {
      if (b->x > a->x) { b->x = a->x; }
      if (b->y > a->y) { b->y = a->y; }
      if (b->z > a->z) { b->z = a->z; }
   }

   void generateChildren() {
      /*
         https://github.com/brandonpelfrey/SimpleOctree/blob/master/Octree.h
         24: Children follow a predictable pattern to make accesses simple.
         25: Here, - means less than 'origin' in that dimension, + means greater than.
         26: child:	0 1 2 3 4 5 6 7
         27: x:      - - - - + + + +
         28: y:      - - + + - - + +
         29: z:      - + - + - + - +
      */

      std::vector< std::shared_ptr<VFCbox> > nnn;
      std::vector< std::shared_ptr<VFCbox> > nnp;
      std::vector< std::shared_ptr<VFCbox> > npn;
      std::vector< std::shared_ptr<VFCbox> > npp;
      std::vector< std::shared_ptr<VFCbox> > pnn;
      std::vector< std::shared_ptr<VFCbox> > pnp;
      std::vector< std::shared_ptr<VFCbox> > ppn;
      std::vector< std::shared_ptr<VFCbox> > ppp;

      for (auto &box : this->objs) {
         if (box->center.x < this->box->center.x && box->center.y < this->box->center.y && box->center.z < this->box->center.z) {
            nnn.push_back(box);
         } else if (box->center.x < this->box->center.x && box->center.y < this->box->center.y) {
            nnp.push_back(box);
         } else if (box->center.x < this->box->center.x && box->center.z < this->box->center.z) {
            npn.push_back(box);
         } else if (box->center.x < this->box->center.x) {
            npp.push_back(box);
         } else if (box->center.y < this->box->center.y && box->center.z < this->box->center.z) {
            pnn.push_back(box);
         } else if (box->center.y < this->box->center.y) {
            pnp.push_back(box);
         } else if (box->center.z < this->box->center.z) {
            ppn.push_back(box);
         } else { // x, y, and z are >= to center
            ppp.push_back(box);
         }
      }
      this->children[0] = std::make_shared<VFCoct>(nnn);
      this->children[1] = std::make_shared<VFCoct>(nnp);
      this->children[2] = std::make_shared<VFCoct>(npn);
      this->children[3] = std::make_shared<VFCoct>(npp);
      this->children[4] = std::make_shared<VFCoct>(pnn);
      this->children[5] = std::make_shared<VFCoct>(pnp);
      this->children[6] = std::make_shared<VFCoct>(ppn);
      this->children[7] = std::make_shared<VFCoct>(ppp);
   }

};

#endif
