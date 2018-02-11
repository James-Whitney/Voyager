#include "include/Scene.h"

#include <iostream>

#include <voyager-render/include/Renderable.h>

using namespace std;

void Scene::apply(shared_ptr<Application> app) {

   for (int i = 0; i < this->entities.size(); ++i) {
      shared_ptr<Entity> entity = this->entities.at(i);
      app->getThings()[entity->getId()] = entity;
   }

   for (int i = 0; i < this->components.size(); ++i) {
      shared_ptr<Component> component = this->components.at(i);

      if (dynamic_pointer_cast<Renderable>(component)) {
         app->getRenderEngine()->registerComponent(component);
      }

   }

}

void Scene::dump() {
   cout << "<Scene>" << endl;
   cout << this->shapes.size() << " shapes" << endl;
   cout << this->ubers.size() << " ubers" << endl;
   cout << this->entities.size() << " entities" << endl;
   cout << this->components.size() << " components" << endl;
   cout << "</Scene>" << endl;
}
