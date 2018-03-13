#include "include/TurretComponent.h"

void TurretComponent::init() {

}

void TurretComponent::update(double delta_time) {
   if (active) {
      logicUpdate(delta_time);
      cameraUpdate(cameraHeight);
   }
}

void TurretComponent::logicUpdate(double delta_time) {

}
