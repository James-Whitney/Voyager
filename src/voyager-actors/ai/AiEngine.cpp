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

void AiEngine::execute(double delta_time) {
   log("--< Ai Engine >------------------------------------");
   for (int i = 0; i < this->components.size(); ++i) {
      stringstream ss;
      ss << "Component " << i;
      log(ss.str());
      auto component = this->components.at(i);
      this->runBrain(delta_time, static_pointer_cast<BrainComponent>(component));
   }
   log("---------------------------------------------------");
}

void AiEngine::runBrain(double delta_time, std::shared_ptr<BrainComponent> brain) {
   brain->update(delta_time);
}
