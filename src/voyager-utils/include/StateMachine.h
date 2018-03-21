#pragma once
#ifndef _STATEMACHINE_H
#define _STATEMACHINE_H

#include <memory>
#include <set>

#include "State.h"

class StateMachine :
   public std::enable_shared_from_this<StateMachine>,
   private std::set< std::shared_ptr<State> >
{

public:

   bool running() { return this->cur_state != nullptr; }

   virtual void build() = 0;

   void start(std::shared_ptr<State> initial_state);
   void run(double delta_time);
   void stop();

   void addState(std::shared_ptr<State> state);
   void setState(std::shared_ptr<State> state);

private:

   std::shared_ptr<State> cur_state = nullptr;

   bool contains(std::shared_ptr<State> state);

};

#endif
