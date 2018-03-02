#include "include/PlayerComponent.h"

#include <iostream>

#define PI 3.1415926

void PlayerComponent::init() {

}

glm::vec3 PlayerComponent::getPosition() {
   return bulletToGlm(entity->getTransform()->getOrigin());
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

void PlayerComponent::stationSelectionCheck() {
   if (glfwGetKey(window->getHandle(), GLFW_KEY_1 ) == GLFW_PRESS) {
      active = true;
      if (mounted != nullptr)
         mounted->deactivate();  
      mounted = nullptr;
   }
   else if (glfwGetKey(window->getHandle(), GLFW_KEY_2 ) == GLFW_PRESS) {
      active = false;
      if (mounted != nullptr) {
         mounted->deactivate();
      }
      mounted = helm;
      mounted->activate();
   }
}

void PlayerComponent::update(double delta_time) {
   //cout << "Player Update" << endl;
   stationSelectionCheck();
   if (active) {
      positionUpdate(delta_time);
      cameraUpdate();
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
   //Space goes up
   /*
   //Shift goes down
   if(glfwGetKey(window->getHandle(), GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){
      movement -= Y_AXIS * deltaTime;
   }*/
   
   if(length(movement) != 0)
   {
      //cout << "MOVING" << endl;
      movement = glm::normalize(movement);
      movement *= (delta_time * speed);
      btVector3 bullet_force = glmToBullet(movement).rotate(btVector3(0, 1, 0), -(ship->getRotation()));
      physicsComponent->getBody()->applyCentralForce(bullet_force);
   }
   else {
      //cout << "DAMPING" << endl;
      physicsComponent->getBody()->applyDamping(delta_time);
   }
   
   //physicsComponent->getBody()->setLinearVelocity(bullet_force);

   //physicsComponent->getBody()->setAngularVelocity(ship->getPhysics()->getBody()->getAngularVelocity());
}