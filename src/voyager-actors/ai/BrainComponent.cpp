#include "../include/ai/BrainComponent.h"

#include <iostream>

using namespace std;

BrainComponent::BrainComponent(shared_ptr<Brain> brain) : Component(), brain(brain) {}

void BrainComponent::init() {
   ent_ptr_t me = this->getEntity();
   assert(me != nullptr);
   this->brain->start(me);
}

void BrainComponent::update(double delta_time) {
   this->brain->run(delta_time);
}
