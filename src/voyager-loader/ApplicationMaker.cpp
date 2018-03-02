#include "include/ApplicationMaker.h"

using namespace std;

shared_ptr<Application> make_application(shared_ptr<VoyagerConfig> config) {

   // make the application
   shared_ptr<Application> app = make_shared<Application>();
   app->setConfig(config);

   // render engine
   shared_ptr<RenderEngine> render = make_shared<RenderEngine>();
   render->setResourceDir(config->getResourceDir());
   shared_ptr<Camera> camera = make_shared<Camera>();
   camera->setWindow(app->getWindowManager());
   render->setCamera(camera);
   render->setWindow(app->getWindowManager());
   app->setRenderEngine(static_pointer_cast<Engine>(render));

   // physics engine
   shared_ptr<PhysicsEngine> physics = make_shared<PhysicsEngine>();
   physics->init();
   app->setPhysicsEngine(static_pointer_cast<Engine>(physics));

   //actor engine
   shared_ptr<ActorEngine> actors = make_shared<ActorEngine>();
   app->setActorEngine(static_pointer_cast<Engine>(actors));

   // done
   return app;

}
