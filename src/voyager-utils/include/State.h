#pragma once
#ifndef _STATE_H
#define _STATE_H

#include <memory>

class StateMachine;

class State {

public:

   virtual void onStart(std::shared_ptr<State> prev) = 0;
   virtual void run(double delta_time) = 0;
   virtual void onEnd(std::shared_ptr<State> next) = 0;

private:

};

#endif
