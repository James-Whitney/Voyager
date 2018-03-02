#include "include/StationComponent.h"

btScalar StationComponent::getRotation() {
   btScalar yaw, pitch, roll;
   entity->getTransform()->getBasis().getEulerYPR(yaw, pitch, roll);
   //printf("Y: %f P: %f R: %f\n", yaw, pitch, roll);
   if (yaw != 0.0) {
      pitch *= -1;
      pitch += yaw;
   }
   return btScalar(pitch);
}

void StationComponent::cameraUpdate(btScalar cameraHeight) {
   double stationAngle = getRotation();
   double deltaStationAngle = stationAngle - prev_StationAngle;
   prev_StationAngle = stationAngle;

   glm::vec3 stationLoc = bulletToGlm(this->getEntity()->getTransform()->getOrigin());
   camera->update(stationLoc + glm::vec3(0, cameraHeight, 0), deltaStationAngle);
   return;
}