#include "include/Renderable.h"

using namespace std;

void Renderable::init() {
   if (this->shape != nullptr) {
      this->shape->init();
   }
}

void Renderable::update(double delta_time) {

}

void Renderable::setShape(string obj_path) {

   vector<tinyobj::shape_t> obj_shape;
   vector<tinyobj::material_t> obj_mat;

   string err;
   if (!tinyobj::LoadObj(obj_shape, obj_mat, err, obj_path.c_str())) {
      cerr << "Failed to load " << obj_path << ": " << err << endl;
   }

   this->shape = make_shared<Shape>();
   this->shape->createShape(obj_shape[0]);
   this->shape->measure();

}
