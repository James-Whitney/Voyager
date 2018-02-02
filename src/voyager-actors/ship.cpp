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
   glm::vec3 currPos = transform->getPosition();
   angle += deltaAngle;

   glm::vec3 currDir = glm::vec3(sin(transform->getRoll()), 0, cos(transform->getRoll()));

   glm::vec3 deltaMovement = (currDir * deltaPos);

   currPos += (currDir * deltaPos);
   
   setTransform(std::make_shared<Transform>
   (currPos, glm::vec3(2.0, 0.25, 2.0), 3.1415926 / 2, 3.1415926 / 2, angle));

}