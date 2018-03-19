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

void Renderable::renderableInit(std::string resource_dir) {

}

void Renderable::update(double delta_time) {

}

void Renderable::draw(std::shared_ptr<Program> prog) {
   for (std::shared_ptr<Shape> shape : this->getMesh()) {
      shape->draw(prog);
   }
}
