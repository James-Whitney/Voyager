#include "include/ShipComponent.h"
#include <iostream>

#define PI 3.14159

void ShipComponent::init() {

}

void ShipComponent::update(double delta_time) {
   moveShip(delta_time);
}

void ShipComponent::moveShip(float delta_time) {
   float deltaPos = 0;
   float deltaHeight = 0;
   float deltaAngle = 0;

   const float turnSpeed = 100000.0;
   const float flightSpeed = 500000.0;
   const float riseSpeed = 500000.0;

   physicsComponent->getBody()->applyDamping(delta_time);

   // Move forward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_UP ) == GLFW_PRESS) {
      deltaPos += flightSpeed * delta_time;

   }
   // Move backward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
      deltaPos -= flightSpeed * delta_time;
   }

   // Move left
   if (glfwGetKey(window->getHandle(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
      deltaAngle += turnSpeed * delta_time;

   }
   // Move right
   if (glfwGetKey(window->getHandle(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
      deltaAngle -= turnSpeed * delta_time;
   }

   // move up
   if (glfwGetKey(window->getHandle(), GLFW_KEY_R ) == GLFW_PRESS) {
      deltaHeight += riseSpeed * delta_time;
   }

   // move down
   if (glfwGetKey(window->getHandle(), GLFW_KEY_F ) == GLFW_PRESS) {
      deltaHeight -= riseSpeed * delta_time;
   }


   btScalar yaw, pitch, roll;
   entity->getTransform()->getBasis().getEulerYPR(yaw, pitch, roll);
   if (yaw != 0.0) {
      pitch *= -1;
      deltaPos *= -1;
   }

   //apply force for forward/backward movement
   if (deltaPos != 0.0) {
      btVector3 currDir = btVector3(1.0, 0.0, 0);
      currDir = currDir.rotate(btVector3(0, 1.0, 0), pitch) * deltaPos;
      physicsComponent->getBody()->applyCentralForce(currDir);

   }

   if (deltaHeight != 0.0) {
      btVector3 moveHeight = btVector3(0.0, 1.0, 0.0) * deltaHeight;
      physicsComponent->getBody()->applyCentralForce(moveHeight);
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
