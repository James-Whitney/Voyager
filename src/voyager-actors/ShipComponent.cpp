#include "include/ShipComponent.h"
#include <iostream>

void ShipComponent::init() {
   
}

void ShipComponent::update(double delta_time) {
   moveShip(delta_time);
}

void ShipComponent::moveShip(float delta_time) {
   float deltaPos = 0;
   float deltaAngle = 0;

   const float turnSpeed = 5000.0;
   const float flightSpeed = 50000.0;

   physicsComponent->getBody()->applyDamping(delta_time);

   // Move forward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_UP ) == GLFW_PRESS) {
      deltaPos += flightSpeed * delta_time;
      
   }
   // Move backward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
      deltaPos -= flightSpeed * delta_time;
   }

   // Move forward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
      deltaAngle += turnSpeed * delta_time;
      
   }
   // Move backward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
      deltaAngle -= turnSpeed * delta_time;
   }
   btQuaternion btQuad = entity->getTransform()->getRotation();
   btScalar yaw = btQuad.getAngle();
   cout << "Yaw: " << yaw << endl;
   //apply force for forward/backward movement
   if (deltaPos != 0.0) {
      btVector3 currDir = btVector3(1.0, 0, 0);
      currDir = currDir.rotate(btVector3(0, 1.0, 0), yaw) * deltaPos;
      physicsComponent->getBody()->applyCentralForce(currDir);
      
   }
   //apply torque for turning
   if (deltaAngle != 0.0) {
      physicsComponent->getBody()->applyTorque(btVector3(0,deltaAngle,0));
   }
   //physicsComponent->getBody()->applyTorque(btVector3(0,1,0));
   //cout << "DeltaPos: " << deltaPos << " DeltaAngle: " << deltaAngle << " Yaw: " << yaw << endl; 

   
   //fprintf(stderr, "Ship_Angle: %f\n", yaw);
   //fprintf(stderr, "getLinearSleepingThreshold : %f\n", physicsComponent->getBody()->getLinearSleepingThreshold());
   //fprintf(stderr, "getAngularSleepingThreshold: %f\n", physicsComponent->getBody()->getAngularSleepingThreshold());


	
/*
   btVector3 bullet_force = deltaPos * transform->getRotation();
   physicsComponent->getBody()->applyCentralForce(bullet_force);

   glm::vec3 currPos = transform->getOrigin();
   angle += deltaAngle;

   glm::vec3 currDir = glm::vec3(sin(transform->getRoll()), 0, cos(transform->getRoll()));

   glm::vec3 deltaMovement = (currDir * deltaPos);

   currPos += (currDir * deltaPos);
*/
}