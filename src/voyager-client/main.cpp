#include <iostream>
#include <memory>
#include <string>

#include <voyager-core/include/All.h>

#include <voyager-loader/include/VoyagerConfigLoader.h>
#include <voyager-loader/include/ApplicationMaker.h>
#include <voyager-loader/include/Scene.h>
#include <voyager-loader/include/SceneLoader.h>
#include <voyager-utils/include/Time.h>

#define RESOURCE_DIR "resources/"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

using namespace glm;
using namespace std;

int main(int argc, char *argv[]) {

   // Get the path to the config file
   if (argc < 2) {
      cerr << "No config file specified, please pass the path to the config file on the command line" << endl;
      exit(1);
   }
   string config_path(argv[1]);

   // make the application
   VoyagerConfigLoader config_loader;
   shared_ptr<VoyagerConfig> config = config_loader.load(config_path);
   shared_ptr<Application> app = make_application(config);
   // load the scene
   double start_scene_load = currentTimeMs();
   SceneLoader scene_loader(config->getResourceDir());
   shared_ptr<Scene> scene = scene_loader.load(config->getResourceDir()
      + "/templates/example.scene.json");
   scene->apply(app);
   scene = nullptr;
   cout << "Scene loaded in " << currentTimeMs() - start_scene_load << "ms" << endl;

   // run it
   app->run();
}
