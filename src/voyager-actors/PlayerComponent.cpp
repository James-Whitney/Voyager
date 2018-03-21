#include "include/PlayerComponent.h"

#include <iostream>

#define PI 3.1415926

void PlayerComponent::init() {

}

void PlayerComponent::setTurret(std::shared_ptr<StationComponent> turret, int i) {
   switch (i) {
      case 0:
         turret0 = turret;
      break;
      case 1:
         turret1 = turret;
      break;
   }
}

glm::vec3 PlayerComponent::getPosition() {
   return bulletToGlm(entity->getTransform()->getOrigin());
}

void PlayerComponent::collisionCheck() {
   // std::vector< std::shared_ptr<Entity> >  collisionList = getEntity()->getCollideList();
   // for (auto &collision: collisionList) {

   // }
}

btScalar PlayerComponent::getRotation() {
   btScalar yaw, pitch, roll;
   entity->getTransform()->getBasis().getEulerYPR(yaw, pitch, roll);
   //printf("Y: %f P: %f R: %f\n", yaw, pitch, roll);
   if (yaw != 0.0) {
      pitch *= -1;
      pitch += yaw;
   }
   return btScalar(pitch);
}

std::shared_ptr<StationComponent> PlayerComponent::attemptMount() {
   std::shared_ptr<StationComponent> nearestMount = nullptr;
   btScalar nearestDistance = 99999999;
   btScalar distance;
   btScalar mountDistance = 5;

   //CHECK HELM
   distance = (helm->getEntity()->getTransform()->getOrigin() - getEntity()->getTransform()->getOrigin()).length();
   if ( (distance < mountDistance) && (distance < nearestDistance) ) {
      nearestMount = helm;
      nearestDistance = distance;
   }
   //CHECK TURRET0
   distance = (turret0->getEntity()->getTransform()->getOrigin() - getEntity()->getTransform()->getOrigin()).length();
   if ( (distance < mountDistance) && (distance < nearestDistance) ) {
      nearestMount = turret0;
      nearestDistance = distance;
   }
   //CHECK TURRET1
   distance = (turret1->getEntity()->getTransform()->getOrigin() - getEntity()->getTransform()->getOrigin()).length();
   if ( (distance < mountDistance) && (distance < nearestDistance) ) {
      nearestMount = turret1;
      nearestDistance = distance;
   }
   return nearestMount;
}


void PlayerComponent::stationSelectionCheck() {
   if ((glfwGetKey(window->getHandle(), GLFW_KEY_E ) == GLFW_PRESS) && !E_DeBounce) {
      E_DeBounce = true;
      if (mounted != nullptr){
         mounted->deactivate();
         mounted = nullptr;
         active = true;
      }
      else {
         mounted = attemptMount();
         if (mounted != nullptr) {
            mounted->activate();
            active = false;
         }
      }
   }
   if ((glfwGetKey(window->getHandle(), GLFW_KEY_E ) == GLFW_RELEASE) && E_DeBounce) {
      E_DeBounce = false;
   }
}

void PlayerComponent::update(double delta_time) {
   //cout << "Player Update" << endl;
   stationSelectionCheck();
   if (active) {
      positionUpdate(delta_time);
      cameraUpdate();
      collisionCheck();
   }
}

void PlayerComponent::cameraUpdate() {
   const float cameraHeight = 2.0;

   double playerAngle = getRotation();
   double deltaPlayerAngle = playerAngle - prev_playerAngle;
   prev_playerAngle = playerAngle;

   glm::vec3 playerLoc = bulletToGlm(this->getEntity()->getTransform()->getOrigin());
   camera->update(playerLoc + glm::vec3(0, cameraHeight, 0), deltaPlayerAngle);
   return;
}

void PlayerComponent::positionUpdate(float delta_time) {
   glm::vec3 movement = glm::vec3(0,0,0);

   const float speed = 50000;

   physicsComponent->getBody()->applyGravity();


   glm::vec3 lookDir = camera->getLookAt(false);
   lookDir = glm::vec3(lookDir.x, 0, lookDir.z);
   // Move forward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_W ) == GLFW_PRESS) {
      movement += lookDir;
   }
   // Move backward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_S ) == GLFW_PRESS) {
      movement -= lookDir;
   }
   // Strafe right
   if (glfwGetKey(window->getHandle(), GLFW_KEY_A ) == GLFW_PRESS) {
      movement -= glm::normalize(glm::cross( lookDir, glm::vec3(0, 1, 0)));
   }
   // Strafe left
   if (glfwGetKey(window->getHandle(), GLFW_KEY_D ) == GLFW_PRESS) {
      movement += glm::normalize(glm::cross( lookDir, glm::vec3(0, 1, 0)));
   }

   if(length(movement) != 0) {
      movement = glm::normalize(movement);
      movement *= (delta_time * speed);
      btVector3 bullet_force = glmToBullet(movement).rotate(btVector3(0, 1, 0), -(ship->getRotation()));
      physicsComponent->getBody()->applyCentralForce(bullet_force);
   }
   else {
      physicsComponent->getBody()->applyDamping(delta_time);
   }
}
