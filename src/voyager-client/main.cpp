#include <iostream>
#include <memory>
#include <string>

#include <voyager-core/include/All.h>
#include <voyager-loader/include/VoyagerConfigLoader.h>

using namespace std;

int main(int argc, char *argv[]) {

   // Get the path to the config file
   if (argc < 2) {
      cerr << "No config file specified, please pass the path to the config file on the command line" << endl;
      exit(1);
   }
   string config_path(argv[1]);

   // load the config file
   VoyagerConfigLoader loader;
   shared_ptr<VoyagerConfig> config = loader.load(config_path);

   // initialize the Application
   shared_ptr<Application> app = make_shared<Application>(config);

}
