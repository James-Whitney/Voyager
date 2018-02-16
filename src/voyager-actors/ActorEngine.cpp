#include "include/ActorEngine.h"

void ActorEngine::init() {

}

void ActorEngine::execute(double delta_time) {
   for (int i = 0; i < components.size(); i++) {
      components[i]->update(delta_time);
   }
}