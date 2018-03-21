#include "../include/ai/Brain.h"

#include <iostream>

using namespace std;

Brain::Brain(shared_ptr<Entity> player) :
   StateMachine(), player(player)
{
   // this->state_do_nothing = make_shared<DoNothing>(player);
   // this->state_chase = make_shared<Chase>(player);
}

void Brain::build() {
   assert(this->state_do_nothing != nullptr);
   assert(this->state_chase != nullptr);

   this->addState(this->state_do_nothing);
   this->state_do_nothing->setEnemy(this->enemy);
   shared_ptr<Brain> me = static_pointer_cast<Brain>(this->shared_from_this());
   this->state_do_nothing->setBrain(me);
   this->addState(this->state_chase);
   this->state_chase->setEnemy(this->enemy);
   this->state_chase->setBrain(me);
}

void Brain::start(shared_ptr<Entity> enemy) {
   cout << "starting brain" << endl;
   this->enemy = enemy;
   this->build();
   StateMachine::start(this->state_do_nothing);
}

void Brain::startChasing() {
   if (this->doing_nothing) {
      this->setState(this->state_chase);
      this->doing_nothing = false;
   }
}

void Brain::startDoingNothing() {
   if (!this->doing_nothing) {
      this->setState(this->state_do_nothing);
      this->doing_nothing = true;
   }
}

btVector3 Brain::getNavTarget(shared_ptr<NavMap> nav_map) {
   // do an A* search along enough of the map to find
   wpt_ptr_t start = nav_map->getNearestWaypoint(this->enemy->getTransform()->getOrigin());
   wpt_ptr_t end = nav_map->getNearestWaypoint(this->player->getTransform()->getOrigin());
   int max_depth = 3;
   wpt_list_t route = nav_map->navigate(start, end, max_depth);
   assert(route.size() > 0);
   wpt_ptr_t target = route.at(0);
   return btVector3(
      target->getLocation().x,
      target->getLocation().y,
      target->getLocation().z
   );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Brain State                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////

BrainState::BrainState(ent_ptr_t player) :
   State(), player(player)
{}

float BrainState::getDistToPlayer() {
   btVector3 p_pos = this->player->getTransform()->getOrigin();
   // cout << "player at (" << p_pos.getX() << ", " << p_pos.getY() << ", " << p_pos.getZ() << ")" << endl;
   btVector3 e_pos = this->enemy->getTransform()->getOrigin();
   // cout << "enemy at (" << e_pos.getX() << ", " << e_pos.getY() << ", " << e_pos.getZ() << ")" << endl;
   float d = (p_pos - e_pos).length();
   // cout << "d = " << d << endl;
   return d;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Do Nothing                                                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////

float DoNothing::START_CHASING_DISTANCE = 300;

void DoNothing::onStart(shared_ptr<State> prev) {
   // cout << "WE ARE DOING NOTHING" << endl;
}

void DoNothing::run(double delta_time) {
   // cout << "-- DOING NOTHING --" << endl;
   float d = this->getDistToPlayer();
   if (d > DoNothing::START_CHASING_DISTANCE) {
      // continue doing nothing
      this->doNothing(delta_time, d);
   } else {
      // transition to chasing
      this->brain->startChasing();
   }
}

void DoNothing::onEnd(shared_ptr<State> next) {

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Chase                                                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////

float Chase::START_DOING_NOTHING_DISTANCE = 400;

void Chase::onStart(shared_ptr<State> prev) {
   // cout << "LET THE CHASE BEGIN!!!" << endl;
}

void Chase::run(double delta_time) {
   // cout << "-- CHASING --" << endl;
   float d = this->getDistToPlayer();
   if (d < Chase::START_DOING_NOTHING_DISTANCE) {
      // continue chasing
      this->chase(delta_time, d);
   } else {
      // start doing nothing
      this->brain->startDoingNothing();
   }
}

void Chase::onEnd(shared_ptr<State> next) {

}
