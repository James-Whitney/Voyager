#include "include/Ship.h"

void Ship::update(double delta_time) {
   moveShip(delta_time);
}

void Ship::moveShip(float delta_time) {
   float deltaPos = 0;
   float deltaAngle = 0;

   const float turnSpeed = 0.01;
   const float flightSpeed = 0.05;

   // Move forward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_LEFT ) == GLFW_PRESS){
      deltaAngle += turnSpeed * delta_time;
   }
   // Move backward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_RIGHT ) == GLFW_PRESS){
      deltaAngle -= turnSpeed * delta_time;
   }
   // Strafe right
   if (glfwGetKey(window->getHandle(), GLFW_KEY_UP ) == GLFW_PRESS){
      deltaPos += flightSpeed * delta_time;
   }
   // Strafe left
   if (glfwGetKey(window->getHandle(), GLFW_KEY_DOWN ) == GLFW_PRESS){
      deltaPos -= flightSpeed * delta_time;
   }
   glm::vec3 currPos = transform->getOrientation()->getPos();
   angle += deltaAngle;
   setTransform(std::make_shared<Transform>(currPos, 
                                             glm::vec3(sin(angle), 0, cos(angle)), 
                                             2));

   glm::vec3 currDir = transform->getOrientation()->getDir();
   glm::vec3 deltaMovement = (currDir * deltaPos);
   setTransform(std::make_shared<Transform>(currPos + deltaMovement, 
                                             currDir,
                                             2));

}