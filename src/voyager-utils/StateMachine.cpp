#include "include/StateMachine.h"

#include <cassert>
#include <iostream>

using namespace std;

void StateMachine::start(shared_ptr<State> initial_state) {
   assert((!this->running() && this->contains(initial_state)));
   this->cur_state = initial_state;
   this->cur_state->onStart(nullptr);
}

void StateMachine::run() {
   assert(this->cur_state != nullptr);
   this->cur_state->run();
}

void StateMachine::stop() {
   assert(this->running());
   this->cur_state->onEnd(nullptr);
   this->cur_state = nullptr;
}

void StateMachine::addState(shared_ptr<State> state) {
   assert(!this->contains(state));
   this->insert(state);
   // shared_ptr<StateMachine> me = this->shared_from_this();
   // state->setMachine(me);
}

void StateMachine::setState(shared_ptr<State> state) {
   assert(this->contains(state));
   this->cur_state->onEnd(state);
   shared_ptr<State> prev = this->cur_state;
   this->cur_state = state;
   this->cur_state->onStart(prev);
}

bool StateMachine::contains(shared_ptr<State> state) {
   return this->find(state) != this->end();
}
