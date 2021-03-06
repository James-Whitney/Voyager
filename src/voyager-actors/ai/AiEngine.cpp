#include "../include/ai/AiEngine.h"
#include "../include/ai/BrainComponent.h"

#include <sstream>

#define _LOG_AI_ENGINE 1

using namespace std;

static void log(string msg) {
#if _LOG_AI_ENGINE
   cout << msg << endl;
#endif
}

void AiEngine::init() {
   for (int i = 0; i < this->components.size(); ++i) {
      this->components.at(i)->init();
   }
}

void AiEngine::removeFlagged() {
   for (int i = 0; i < components.size(); i++) {
      std::shared_ptr<Component> component = components[i];
      if (component->getRemoveFlag()) {
         components.erase(components.begin() + i);
      }
   }
}

void AiEngine::execute(double delta_time) {
   // log("--< Ai Engine >------------------------------------");
   for (int i = 0; i < this->components.size(); ++i) {
      // stringstream ss;
      // ss << "Component " << i;
      // log(ss.str());
      auto component = this->components.at(i);
      this->runBrain(delta_time, static_pointer_cast<BrainComponent>(component));
   }

   // log("Spawners");
   for (int i = 0; i < this->spawners.size(); ++i) {
      this->spawners.at(i)->update(delta_time);
   }

   // log("---------------------------------------------------");
}

void AiEngine::runBrain(double delta_time, std::shared_ptr<BrainComponent> brain) {
   brain->update(delta_time);
}
void AiEngine::registerComponent(shared_ptr<Component> c) {
   if (dynamic_pointer_cast<Spawner>(c)) {
      this->spawners.push_back(static_pointer_cast<Spawner>(c));
   } else {
      Engine::registerComponent(c);
   }
}
