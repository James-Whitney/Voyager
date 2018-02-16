#include "include/Scene.h"



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
      else if (dynamic_pointer_cast<PhysicsComponent>(component)) {
         app->getPhysicsEngine()->registerComponent(component);
      }
      else if (dynamic_pointer_cast<ActorComponent>(component)) {
         app->getActorEngine()->registerComponent(component);
      }
   }

   //init ship
   shared_ptr<Entity> ship = this->entities.at(0);
   shared_ptr<ShipComponent> shipComponent = static_pointer_cast<ShipComponent>(ship->componentAt(ship->numComponents()-1));
   shipComponent->setWindow(app->getWindowManager());

   //init player
   shared_ptr<Entity> player = this->entities.at(1);
   shared_ptr<PlayerComponent> playerComponent = static_pointer_cast<PlayerComponent>(player->componentAt(player->numComponents()-1));
   playerComponent->setWindow(app->getWindowManager());
   playerComponent->setCamera(static_pointer_cast<RenderEngine>(app->getRenderEngine())->getCamera());
   playerComponent->setShip(shipComponent);

}

void Scene::dump() {
   cout << "<Scene>" << endl;
   cout << this->shapes.size() << " shapes" << endl;
   cout << this->ubers.size() << " ubers" << endl;
   cout << this->entities.size() << " entities" << endl;
   cout << this->components.size() << " components" << endl;
   cout << "</Scene>" << endl;
}
