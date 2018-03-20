#include "../include/ai/Brain.h"

#include <iostream>

using namespace std;

Brain::Brain(shared_ptr<Entity> player) :
   StateMachine(), player(player)
{
   this->state_do_nothing = make_shared<DoNothing>(player);
   this->state_chase = make_shared<Chase>(player);
}

void Brain::build() {
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

   float d = sqrtf(
      powf(p_pos.getX() - e_pos.getX(), 2.0f) +
      powf(p_pos.getY() - e_pos.getY(), 2.0f) +
      powf(p_pos.getZ() - e_pos.getZ(), 2.0f)
   );
   cout << "d = " << d << endl;
   return d;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Do Nothing                                                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////

float DoNothing::START_CHASING_DISTANCE = 300;

void DoNothing::onStart(shared_ptr<State> prev) {
   cout << "WE ARE DOING NOTHING" << endl;
}

void DoNothing::run() {
   cout << "-- DOING NOTHING --" << endl;
   float d = this->getDistToPlayer();
   if (d > DoNothing::START_CHASING_DISTANCE) {
      // continue doing nothing

      // TODO: wander aimlessly

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
   cout << "LET THE CHASE BEGIN!!!" << endl;
}

void Chase::run() {
   cout << "-- CHASING --" << endl;
   float d = this->getDistToPlayer();
   if (d < Chase::START_DOING_NOTHING_DISTANCE) {
      // continue chasing

      // TODO: case the player

   } else {
      // start doing nothing
      this->brain->startDoingNothing();
   }
}

void Chase::onEnd(shared_ptr<State> next) {

}
