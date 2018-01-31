#include "include/Engine.h"

using namespace std;

void Engine::registerComponent(shared_ptr<Component> component) {
   this->components.push_back(component);
}
