#include "../include/ai/Brain.h"

using namespace std;

void Brain::build() {
   this->state_do_nothing = make_shared<DoNothing>();
   this->state_chase = make_shared<Chase>();

   this->addState(this->state_do_nothing);
   this->addState(this->state_chase);
}

shared_ptr<Brain> BrainState::getBrain() {
   return dynamic_pointer_cast<Brain>(this->getMachine());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Do Nothing                                                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////

void DoNothing::onStart(shared_ptr<State> prev) {

}

void DoNothing::run() {

}

void DoNothing::onEnd(shared_ptr<State> next) {

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Chase                                                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////

void Chase::onStart(shared_ptr<State> prev) {

}

void Chase::run() {

}

void Chase::onEnd(shared_ptr<State> next) {

}

