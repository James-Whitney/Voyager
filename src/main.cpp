#include <iostream>
#include <string>

#include "Application.h"

#define RESOURCE_DIR "../resources"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

using namespace std;

shared_ptr<Application> makeApp(string resource_dir) {
   shared_ptr<Application> app = make_shared<Application>(resource_dir);
   shared_ptr<WindowManager> window = make_shared<WindowManager>();

   window->init(WINDOW_WIDTH, WINDOW_HEIGHT);
   window->setEventCallbacks(app.get());
   app->setWindowManager(window);
   return app;
}

int main(int argc, char *argv[]) {

   string resource_dir = argc >= 2 ? argv[1] : RESOURCE_DIR;
   cout << "Resource Directory: " << resource_dir << endl;
   shared_ptr<Application> app = makeApp(resource_dir);

   app->run();

   return 0;
}
