#include "include/Player.h"

#define PI 3.1415926

Player::Player() {

}

void Player::init() {

}

glm::vec3 Player::getPosition() {
   return ship->getTransform()->getOrientation()->getPos() + 
                     transform->getOrientation()->getPos();
}


void Player::update(double delta_time) {
   positionUpdate(delta_time);
   cameraUpdate();
}

void Player::cameraUpdate() {
   double delta_xPos, delta_yPos;
   double curr_xPos, curr_yPos;
   float delta_pitch, delta_yaw;
   int width, height;

   glfwGetFramebufferSize(window->getHandle(), &width, &height);
   glfwGetCursorPos(window->getHandle(), &curr_xPos, &curr_yPos);
   
   delta_xPos = prev_xPos - curr_xPos;
   delta_yPos = prev_yPos - curr_yPos;
   prev_xPos = curr_xPos;
   prev_yPos = curr_yPos;

   delta_pitch = delta_yPos / (height * 2);
   delta_yaw = delta_xPos / width;

   float ship_angle = ship->getShipAngle();
   float deltaShipAngle = ship_angle - prev_shipAngle;
   prev_shipAngle = ship_angle;

   camera->move(delta_pitch, delta_yaw + deltaShipAngle);
   
   camera->setPosition(glm::vec3(0, 1, 0) + getPosition());
   return;
}

void Player::positionUpdate(float delta_time) {
   glm::vec3 movement = glm::vec3(0,0,0);

   const float speed = 0.1;

   glm::vec3 lookDir = camera->getLookAt(false);
   lookDir = glm::vec3(lookDir.x, 0, lookDir.z);
   // Move forward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_W ) == GLFW_PRESS){
      movement += lookDir;
   }
   // Move backward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_S ) == GLFW_PRESS){
      movement -= lookDir;
   }
   // Strafe right
   if (glfwGetKey(window->getHandle(), GLFW_KEY_A ) == GLFW_PRESS){
      movement -= glm::normalize(glm::cross( lookDir, glm::vec3(0, 1, 0)));
   }
   // Strafe left
   if (glfwGetKey(window->getHandle(), GLFW_KEY_D ) == GLFW_PRESS){
      movement += glm::normalize(glm::cross( lookDir, glm::vec3(0, 1, 0)));
   }
   /*
   //Space goes up
   if(glfwGetKey(window->getHandle(), GLFW_KEY_SPACE ) == GLFW_PRESS){
      movement += Y_AXIS * deltaTime;
   }
   //Shift goes down
   if(glfwGetKey(window->getHandle(), GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){
      movement -= Y_AXIS * deltaTime;
   }*/
   if(length(movement) > 0)
   {
      movement = glm::normalize(movement);
   }
   movement *= delta_time * speed;

   glm::vec3 pos = transform->getOrientation()->getPos() + movement;
   if(pos.x > 1)
      pos.x = 1;
   if(pos.x < -1)
      pos.x = -1;
   if(pos.z > 1)
      pos.z = 1;
   if(pos.z < -1)
      pos.z = -1;
   setTransform(std::make_shared<Transform>(pos, glm::vec3(1, 0, 0), 1));
}