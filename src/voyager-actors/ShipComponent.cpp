#include "include/ShipComponent.h"
#include <iostream>

#define PI 3.141592684

void ShipComponent::init() {

}

void ShipComponent::update(double delta_time) {
   moveShip(delta_time);
}

btScalar ShipComponent::getRotation() {
   btScalar yaw, pitch, roll;
   entity->getTransform()->getBasis().getEulerYPR(yaw, pitch, roll);
   if (yaw != 0.0) {
      pitch *= -1;
      pitch += yaw;
   }
   return btScalar(pitch);
}

btVector3 ShipComponent::getRotationVector() {
   btScalar yaw, pitch, roll;
   entity->getTransform()->getBasis().getEulerYPR(yaw, pitch, roll);
   return btVector3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
}

void ShipComponent::turnShip(btScalar angle) {
   if (angle != 0.0) {
      if (forwardThrottle < 0) {
         angle *= -1;
      }
      physicsComponent->getBody()->applyTorque(btVector3(0,angle,0)); 
   }
}

void ShipComponent::raiseShip(btScalar rise) {
   btVector3 moveVert = btVector3(0.0, 1.0, 0.0) * rise;
   physicsComponent->getBody()->applyCentralForce(moveVert);
}

void ShipComponent::moveShip(float delta_time) {
   physicsComponent->getBody()->applyDamping(delta_time);

   btVector3 movement = btVector3(1.0, 0.0, 0) * forwardThrottle * speed * delta_time;
   movement = movement.rotate(btVector3(0, 1.0, 0), getRotation());

   physicsComponent->getBody()->applyCentralForce(movement);
}
