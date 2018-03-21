#include "../include/ai/BrainComponent.h"

#include <iostream>

using namespace std;

BrainComponent::BrainComponent(shared_ptr<Entity> player) : Component() {
   this->brain = make_shared<Brain>(player);
}

void BrainComponent::init() {
   cout << "INITIALIZING BRAIN COMPONENT" << endl;
   ent_ptr_t me = this->getEntity();
   assert(me != nullptr);
   this->brain->start(me);
}

void BrainComponent::update(double delta_time) {
   this->brain->run(delta_time);
}
