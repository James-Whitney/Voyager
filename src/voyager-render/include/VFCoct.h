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
      std::cout << "Pushing " << box.size() << " boxes" << std::endl;

      for (auto &b : box) { this->objs.push_back(b); }
      std::cout << "Size of objs is " << this->objs.size() << std::endl;
      generateBox();
      if (this->objs.size() > 1) {
         generateChildren();
      } else {
         for (auto &child : this->children) { child = NULL; }
      }
   }

   void generateBox() {
      std::cout << "Generating Boxes" << std::endl;

      std::vector<int> idxs;
      vec3 min = vec3(std::numeric_limits<float>::infinity());
      vec3 max = vec3(-1 * std::numeric_limits<float>::infinity());
      vec3 newTempMin;
      vec3 newTempMax;
      for (auto &temp : this->objs) {
         for (auto &i : temp->idxs) { idxs.push_back(i); }

         std::cout << "temp cen: " << temp->center.x << " " << temp->center.y << " " << temp->center.z << std::endl;
         std::cout << "temp min: " << temp->center.x+temp->min.x << " " << temp->center.y+temp->min.y << " " << temp->center.z+temp->min.z << std::endl;
         std::cout << "temp max: " << temp->center.x+temp->max.x << " " << temp->center.y+temp->max.y << " " << temp->center.z+temp->max.z << std::endl;

         newTempMin = temp->center + temp->min;
         newTempMax = temp->center + temp->max;

         if (isinf(min.x) || newTempMin.x < min.x) { min.x = newTempMin.x; }
         if (isinf(min.y) || newTempMin.y < min.y) { min.y = newTempMin.y; }
         if (isinf(min.z) || newTempMin.z < min.z) { min.z = newTempMin.z; }

         if (isinf(max.x) || newTempMax.x > max.x) { max.x = newTempMax.x; }
         if (isinf(max.y) || newTempMax.y > max.y) { max.y = newTempMax.y; }
         if (isinf(max.z) || newTempMax.z > max.z) { max.z = newTempMax.z; }
      }

      std::cout << "min: " << min.x << " " << min.y << " " << min.z << std::endl;
      std::cout << "max: " << max.x << " " << max.y << " " << max.z << std::endl;

      vec3 center = vec3((max.x+min.x)/2.0f,(max.y+min.y)/2.0f,(max.z+min.z)/2.0f);
      float radius = sqrt(pow((max.x-min.x),2)+pow((max.y-min.y),2)+pow((max.z-min.z),2))/2.0f;
      this->box = new VFCbox(center, min, max, radius, &idxs);
      std::cout << "Oct Box is: " << this->box->center.x << " " << this->box->center.y << " " << this->box->center.z << "; " << this->box->radius << std::endl;
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
         std::cout << "box center: " << box->center.x << " " << box->center.y << " "  << box->center.z << std::endl;
      }

      std::cout << "nnn size = " << nnn.size() << std::endl;
      std::cout << "nnp size = " << nnp.size() << std::endl;
      std::cout << "npn size = " << npn.size() << std::endl;
      std::cout << "npp size = " << npp.size() << std::endl;
      std::cout << "pnn size = " << pnn.size() << std::endl;
      std::cout << "pnp size = " << pnp.size() << std::endl;
      std::cout << "ppn size = " << ppn.size() << std::endl;
      std::cout << "ppp size = " << ppp.size() << std::endl;

      nnn.size() > 0 ? this->children[0] = std::make_shared<VFCoct>(nnn) : this->children[0] = NULL;
      nnp.size() > 0 ? this->children[1] = std::make_shared<VFCoct>(nnp) : this->children[1] = NULL;
      npn.size() > 0 ? this->children[2] = std::make_shared<VFCoct>(npn) : this->children[2] = NULL;
      npp.size() > 0 ? this->children[3] = std::make_shared<VFCoct>(npp) : this->children[3] = NULL;
      pnn.size() > 0 ? this->children[4] = std::make_shared<VFCoct>(pnn) : this->children[4] = NULL;
      pnp.size() > 0 ? this->children[5] = std::make_shared<VFCoct>(pnp) : this->children[5] = NULL;
      ppn.size() > 0 ? this->children[6] = std::make_shared<VFCoct>(ppn) : this->children[6] = NULL;
      ppp.size() > 0 ? this->children[7] = std::make_shared<VFCoct>(ppp) : this->children[7] = NULL;
   }

};

#endif
