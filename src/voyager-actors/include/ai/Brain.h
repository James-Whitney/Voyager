#pragma once
#ifndef _BRAIN_H
#define _BRAIN_H

#include <memory>

#include <voyager-utils/include/StateMachine.h>

class DoNothing;
class Chase;

class Brain :
   public std::enable_shared_from_this<Brain>,
   private StateMachine
{
public:
   virtual void build();
protected:
   std::shared_ptr<DoNothing> state_do_nothing;
   std::shared_ptr<Chase> state_chase;
};

////

class BrainState : public State {
public:
   std::shared_ptr<Brain> getBrain();
};

////

class DoNothing : public BrainState {
public:
   void onStart(std::shared_ptr<State> prev);
   void run();
   void onEnd(std::shared_ptr<State> next);
};

////

class Chase : public BrainState {
public:
   void onStart(std::shared_ptr<State> prev);
   void run();
   void onEnd(std::shared_ptr<State> next);
};

#endif
