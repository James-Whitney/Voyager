#include <memory>
#include <string>

#include <voyager-core/include/Application.h>
#include <voyager-core/include/ApplicationType.h>
#include <voyager-core/include/Entity.h>

#include <voyager-render/include/RenderEngine.h>
#include <voyager-render/include/Uber.h>

#include <voyager-physics/include/PhysicsEngine.h>

#include <voyager-actors/include/Player.h>
#include <voyager-actors/include/Ship.h>

#define RESOURCE_DIR "resources/"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

using namespace glm;
using namespace std;

shared_ptr<Entity> make_box(string resource_dir);
shared_ptr<Ship> make_ship(string resource_dir);
shared_ptr<Player> make_player(string resource_dir);


int main(int argc, char *argv[]) {

   // initialize the application
   shared_ptr<Application> app = make_shared<Application>(CLIENT, RESOURCE_DIR);

   // initialize the render engine
   shared_ptr<RenderEngine> render = make_shared<RenderEngine>();
   render->setResourceDir(app->getResourceDir());
   shared_ptr<Camera> camera = make_shared<Camera>();
   render->setCamera(camera);
   render->setWindow(app->getWindowManager());
   app->setRenderEngine(static_pointer_cast<Engine>(render));


   // initialize the physics engine
   shared_ptr<PhysicsEngine> physicsEngine = make_shared<PhysicsEngine>();
   app->setPhysicsEngine(static_pointer_cast<Engine>(physicsEngine));


   // make a box
   shared_ptr<Entity> box = make_box(app->getResourceDir());
   render->registerComponent(box->componentAt(0));

   //make a new ship
   shared_ptr<Ship> ship = make_ship(app->getResourceDir());
   render->registerComponent(ship->componentAt(0));
   physicsEngine->registerComponent(ship->componentAt(1));
   ship->setWindow(app->getWindowManager());
   app->addThing(ship);

   // make a new player
   shared_ptr<Player> player = make_player(app->getResourceDir());
   //render->registerComponent(player->componentAt(0));
   physicsEngine->registerComponent(player->componentAt(1));
   player->setCamera(camera);
   player->setWindow(app->getWindowManager());
   player->setShip(ship);
   app->addThing(player);

   // run the application!
   app->run();

}


shared_ptr<Ship> make_ship(string resource_dir) {
   shared_ptr<Ship> result = make_shared<Ship>();

   //render component
   shared_ptr<Renderable> renderComponent = make_shared<Renderable>();
   renderComponent->setShape(resource_dir + "/cube.obj");
   shared_ptr<Uber> uber = make_shared<CookTorranceUber>
      (1.0f, vec3(0.8f, 0.1f, 0.2f), 0.3f, 0.8f, 0.3f);
   renderComponent->setUber(uber);
   result->add(renderComponent);

   //physics component
   shared_ptr<PhysicsComponent> physicsComponent = make_shared<PhysicsComponent>();
   result->add(physicsComponent);

   //set initial Positon
   result->setTransform(make_shared<Transform>(vec3(0, 0, 0),
                                               vec3(1, 0.25, 1),
                                               0, 0, 0));

   return result;
}

shared_ptr<Player> make_player(string resource_dir) {
   shared_ptr<Player> result = make_shared<Player>();

   //render component

   shared_ptr<Renderable> renderComponent = make_shared<Renderable>();
   renderComponent->setShape(resource_dir + "/cube.obj");
   shared_ptr<Uber> uber = make_shared<CookTorranceUber>
      (1.0f, vec3(0.1f, 0.8f, 0.2f), 0.3f, 0.8f, 0.3f);
   renderComponent->setUber(uber);
   result->add(renderComponent);

   //physcis component
   shared_ptr<PhysicsComponent> physicsComponent = make_shared<PhysicsComponent>();
   result->add(physicsComponent);

   //set initial Positon
   result->setTransform(make_shared<Transform>(vec3(0, 0, 0),
                                               vec3(1, 1, 1),
                                               0, 0, 0));
   return result;
}

shared_ptr<Entity> make_box(string resource_dir) {
   shared_ptr<Entity> result = make_shared<Entity>();
   result->setTransform(make_shared<Transform>(vec3(7, 0, 0),
                                               vec3(1, 1, 1),
                                               0, 0, 0));

   shared_ptr<Renderable> renderable = make_shared<Renderable>();
   renderable->setShape(resource_dir + "/cube.obj");
   result->add(renderable);

   shared_ptr<Uber> uber;

   // uber = make_shared<SimpleUber>(vec3(0.02f, 0.04f, 0.2f),
   //    vec3(0.0f, 0.16f, 0.9f));

   // uber = make_shared<BlinnPhongUber>();

   uber = make_shared<CookTorranceUber>(1.0f, vec3(0.1f, 0.8f, 0.2f), 0.3f,
      0.8f, 0.3f);

   renderable->setUber(uber);

   return result;
}
