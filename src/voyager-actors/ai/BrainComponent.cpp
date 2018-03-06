#include "../include/ai/BrainComponent.h"

using namespace std;

BrainComponent::BrainComponent() {
   this->brain = make_shared<Brain>();
}
