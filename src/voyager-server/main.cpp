#include <memory>
#include <string>

#include <voyager-core/include/Application.h>
#include <voyager-core/include/ApplicationType.h>

#define RESOURCE_DIR "../resources"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

using namespace std;

int main(int argc, char *argv[]) {
   Application app = Application(SERVER, RESOURCE_DIR);
   app.run();
}
