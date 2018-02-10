#include <memory>
#include <string>

#include <iostream>

#include <voyager-core/include/Application.h>
#include <voyager-core/include/ApplicationType.h>

#include <voyager-network/include/ServerNetworkEngine.h>

#define RESOURCE_DIR "../resources"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

using namespace std;

int main(int argc, char *argv[]) {
   shared_ptr<Application> app = make_shared<Application>(SERVER, RESOURCE_DIR);
   //Application app(SERVER, RESOURCE_DIR);

   // Initialize the Network Engine
   shared_ptr<ServerNetworkEngine> networkEngine = make_shared<ServerNetworkEngine>();
   app->setNetworkEngine(static_pointer_cast<Engine>(networkEngine));
   app->run();
}
