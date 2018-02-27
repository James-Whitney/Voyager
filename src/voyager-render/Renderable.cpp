#include "include/Renderable.h"

using namespace std;

void Renderable::init() {
   if (!this->mesh.empty()) {
      for (shared_ptr<Shape> shape : this->mesh) {
         if (shape != nullptr) {
            shape->init();
         }
      }
   }
}

void Renderable::update(double delta_time) {

}
