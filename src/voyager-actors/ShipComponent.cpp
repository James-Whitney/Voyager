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

void ShipComponent::moveShip(float delta_time) {
   float deltaAngle = 0;

   const float turnSpeed = 100000.0;

   physicsComponent->getBody()->applyDamping(delta_time);

   // Move forward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_UP ) == GLFW_PRESS) {
      forwardThrottle += delta_time;
      if (forwardThrottle > max_forwardThrottle) {
         forwardThrottle = max_forwardThrottle;
      }

   }
   // Move backward
   if (glfwGetKey(window->getHandle(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
      forwardThrottle -= delta_time;
      if (forwardThrottle < min_forwardThrottle) {
         forwardThrottle = min_forwardThrottle;
      }
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
      vertThrottle += delta_time;
      if (vertThrottle > max_vertThrottle) {
         vertThrottle = max_vertThrottle;
      }
   }
   // move down
   if (glfwGetKey(window->getHandle(), GLFW_KEY_F ) == GLFW_PRESS) {
      vertThrottle -= delta_time;
      if (vertThrottle < min_vertThrottle) {
         vertThrottle = min_vertThrottle;
      }
   }

   if (glfwGetKey(window->getHandle(), GLFW_KEY_X ) == GLFW_PRESS) {
      forwardThrottle = 0;
   }

   if (glfwGetKey(window->getHandle(), GLFW_KEY_C ) == GLFW_PRESS) {
      vertThrottle = 0;
   }

/*
   btScalar yaw, pitch, roll;
   entity->getTransform()->getBasis().getEulerYPR(yaw, pitch, roll);
   if (yaw != 0.0) {
      pitch *= -1;
      deltaPos *= -1;
   }*/

   btScalar yaw = getRotation();

   btVector3 netForce = btVector3(0, 0, 0);

   //printf("Ship Direction: %f\n", pitch);

   btVector3 currDir = btVector3(1.0, 0.0, 0) * forwardThrottle * 500 * delta_time;
   currDir = currDir.rotate(btVector3(0, 1.0, 0), yaw);
   netForce += currDir;
   

   btVector3 moveHeight = btVector3(0.0, 1.0, 0.0) * vertThrottle * 250 * delta_time;
   netForce += moveHeight;

   physicsComponent->getBody()->applyCentralForce(netForce);

   //apply torque for turning
   if (deltaAngle != 0.0) {
      physicsComponent->getBody()->applyTorque(btVector3(0,deltaAngle,0));
   }

   //printf("F: %f H: %f Y: %f\n", forwardThrottle, vertThrottle, yaw);
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
