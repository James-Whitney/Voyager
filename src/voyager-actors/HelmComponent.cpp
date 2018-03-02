#include "include/HelmComponent.h"

void HelmComponent::init() {

}

void HelmComponent::update(double delta_time) {
   if (active) {
      logicUpdate(delta_time);
      cameraUpdate(cameraHeight);
   }
}

void HelmComponent::logicUpdate(double delta_time) {
   btScalar deltaAngle = 0.0;
   btScalar deltaVert = 0.0;

   //Move Forward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_W ) == GLFW_PRESS) {
      forwardThrottle += delta_time;
      if (forwardThrottle > max_forwardThrottle) {
         forwardThrottle = max_forwardThrottle;
      }
   }
   // Move backward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_S ) == GLFW_PRESS) {
      forwardThrottle -= delta_time;
      if (forwardThrottle < min_forwardThrottle) {
         forwardThrottle = min_forwardThrottle;
      }
   }
   // move up
   if (glfwGetKey(window->getHandle(), GLFW_KEY_R ) == GLFW_PRESS) {
      deltaVert += riseSpeed * delta_time;
   }
   // move down
   if (glfwGetKey(window->getHandle(), GLFW_KEY_F ) == GLFW_PRESS) {
      deltaVert -= riseSpeed * delta_time;
   }
   //stop horizontal movement
   if (glfwGetKey(window->getHandle(), GLFW_KEY_X ) == GLFW_PRESS) {
      forwardThrottle = 0;
   }
   // Move left
   if (glfwGetKey(window->getHandle(), GLFW_KEY_A ) == GLFW_PRESS) {
      deltaAngle += turnSpeed * delta_time;

   }
   // Move right
   if (glfwGetKey(window->getHandle(), GLFW_KEY_D ) == GLFW_PRESS) {
      deltaAngle -= turnSpeed * delta_time;
   }
   ship->setForwardThrottle(forwardThrottle);
   ship->turnShip(deltaAngle);
   ship->raiseShip(deltaVert);
}
