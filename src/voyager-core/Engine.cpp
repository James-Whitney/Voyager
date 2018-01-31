#include "include/Engine.h"

using namespace std;

template <typename C>
void Engine<C>::registerComponent(shared_ptr<C> component) {
   this->components.push_back(component);
}
