#include <memory>
#include <string>

#include <voyager-core/include/Application.h>
#include <voyager-core/include/ApplicationType.h>
#include <voyager-core/include/Entity.h>
#include <voyager-render/include/RenderEngine.h>
#include <voyager-render/include/Uber.h>

#define RESOURCE_DIR "resources/"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

using namespace glm;
using namespace std;

shared_ptr<Entity> make_box(string resource_dir);

int main(int argc, char *argv[]) {

   // initialize the application
   shared_ptr<Application> app = make_shared<Application>(CLIENT, RESOURCE_DIR);

   // initialize the render engine
   shared_ptr<RenderEngine> render = make_shared<RenderEngine>();
   render->setResourceDir(app->getResourceDir());
   render->setCamera(make_shared<Camera>());
   render->setWindow(app->getWindowManager());
   app->setRenderEngine(static_pointer_cast<Engine>(render));

   // make a box
   shared_ptr<Entity> box = make_box(app->getResourceDir());
   render->registerComponent(box->componentAt(0));

   // run the application!
   app->run();

}

shared_ptr<Entity> make_box(string resource_dir) {
   shared_ptr<Entity> result = make_shared<Entity>();
   result->setTransform(
      make_shared<Transform>(
         vec3(7, 0, 0),
         vec3(1, 0, 0),
         1
      )
   );

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
