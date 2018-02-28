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

void PlayerComponent::update(double delta_time) {
   //cout << "Player Update" << endl;
   positionUpdate(delta_time);
   cameraUpdate();
   
}

void PlayerComponent::cameraUpdate() {
   double delta_xPos, delta_yPos;
   double curr_xPos, curr_yPos;
   float delta_pitch, delta_yaw;
   int width, height;

   const float cameraHeight = 2.0;

   glfwGetFramebufferSize(window->getHandle(), &width, &height);
   glfwGetCursorPos(window->getHandle(), &curr_xPos, &curr_yPos);
   
   delta_xPos = prev_xPos - curr_xPos;
   delta_yPos = prev_yPos - curr_yPos;
   prev_xPos = curr_xPos;
   prev_yPos = curr_yPos;

   delta_pitch = delta_yPos / (height * 2);
   delta_yaw = delta_xPos / width;


   double playerAngle = getRotation();
   double deltaPlayerAngle = playerAngle - prev_playerAngle;
   prev_playerAngle = playerAngle;

   //fprintf(stderr, "delta_pitch: %f, delta_yaw: %f\n", delta_pitch, delta_yaw);
   camera->move(delta_pitch, delta_yaw + deltaPlayerAngle);// + deltaShipAngle);
   printf("PLAYERANGLE: %f\n", playerAngle);
   
   glm::vec3 playerLoc = bulletToGlm(this->getEntity()->getTransform()->getOrigin());
   camera->setPosition(playerLoc + glm::vec3(0, cameraHeight, 0));

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