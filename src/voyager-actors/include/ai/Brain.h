#pragma once
#ifndef _BRAIN_H
#define _BRAIN_H

#include <memory>

#include <voyager-core/include/Entity.h>
#include <voyager-utils/include/StateMachine.h>

#include "NavMap.h"

class DoNothing;
class Chase;

class Brain :
   public StateMachine
{
public:
   Brain(std::shared_ptr<Entity> player);
   virtual void build() override;
   void start(std::shared_ptr<Entity> enemy);

   void startChasing();
   void startDoingNothing();

   btVector3 getNavTarget(std::shared_ptr<NavMap> nav_map);

protected:

   // states
   std::shared_ptr<DoNothing> state_do_nothing;
   std::shared_ptr<Chase> state_chase;

   // entities
   std::shared_ptr<Entity> enemy;
   std::shared_ptr<Entity> player;

private:
   bool doing_nothing = true;
};

////

class BrainState : public State {
public:
   BrainState(std::shared_ptr<Entity> player);
   void setEnemy(std::shared_ptr<Entity> enemy) { this->enemy = enemy; }
   void setBrain(std::shared_ptr<Brain> brain) { this->brain = brain; }
   float getDistToPlayer();
protected:
   std::shared_ptr<Entity> enemy;
   std::shared_ptr<Entity> player;
   std::shared_ptr<Brain> brain;
};

////

class DoNothing : public BrainState {
public:

   static float START_CHASING_DISTANCE;

   DoNothing(std::shared_ptr<Entity> player) : BrainState(player) {};

   void onStart(std::shared_ptr<State> prev);
   void run(double delta_time);
   void onEnd(std::shared_ptr<State> next);

protected:
   virtual void doNothing(double delta_time, float distance_to_player) = 0;

};

////

class Chase : public BrainState {
public:

   static float START_DOING_NOTHING_DISTANCE;

   Chase(std::shared_ptr<Entity> player) : BrainState(player) {};

   void onStart(std::shared_ptr<State> prev);
   void run(double delta_time);
   void onEnd(std::shared_ptr<State> next);

protected:
   virtual void chase(double delta_time, float distance_to_player) = 0;
};

#endif
