#pragma once
#ifndef _STATE_H
#define _STATE_H

#include <memory>

class StateMachine;

class State {

public:

   std::shared_ptr<StateMachine> getMachine() { return this->machine; }
   void setMachine(std::shared_ptr<StateMachine> machine) { this->machine = machine; }

   virtual void onStart(std::shared_ptr<State> prev) = 0;
   virtual void run() = 0;
   virtual void onEnd(std::shared_ptr<State> next) = 0;

private:

   std::shared_ptr<StateMachine> machine;

};

#endif
