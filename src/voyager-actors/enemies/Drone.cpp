#include "../include/enemies/Drone.h"

using namespace std;

shared_ptr<PhysicsComponent> DronePhysicsProperties::toPhysicsComponent(shared_ptr<Entity> ent) {
   DronePhysicsProperties props;
   shared_ptr<PhysicsComponent> phys = make_shared<PhysicsComponent>();

   // build values
   int world = 1; // always in the outer world
   btVector3 position = ent->getTransform()->getOrigin();
   btQuaternion rotation = ent->getTransform()->getRotation();
   btVector3 velocity = btVector3(0, 0, 0);
   btBoxShape *shape = new btBoxShape(btVector3(props.box_size, props.box_size, props.box_size));

   // initialize the component
   phys->initRigidBody(world, ent, shape, props.mass, position, rotation, velocity, props.friction);
   phys->getBody()->setDamping(props.lin_damp, props.ang_damp);

   return phys;
}

Drone::Drone(shared_ptr<Scene> scene, shared_ptr<NavMap> nav_map, shared_ptr<btTransform> trans) :
   Entity(),
   nav_map(nav_map)
{
   // transform
   this->setTransform(trans);
   this->setScale(make_shared<btVector3>(4, 4, 4));

   // renderable
   this->renderable = make_shared<Renderable>();
   this->renderable->setMesh(scene->meshes[1]);
   shared_ptr<Uber> uber = scene->ubers[6];
   assert(uber != nullptr);
   this->renderable->setUber(uber);
   this->renderable->setCullStatus(false);

   // brain
   shared_ptr<DroneBrain> drone_brain = make_shared<DroneBrain>(nav_map->getPlayer());
   this->brain = make_shared<BrainComponent>(drone_brain);
}

void Drone::initPhysics() {
   shared_ptr<Entity> me = this->shared_from_this();
   assert(me != nullptr);
   this->physics = DronePhysicsProperties::toPhysicsComponent(me);
}

void Drone::linkComponents() {
   this->add(this->renderable);
   this->add(this->brain);
   this->add(this->physics);

   this->preparePhysicsComponent(this->shared_from_this());
}

DroneBrain::DroneBrain(shared_ptr<Entity> player) : Brain(player) {
   this->state_do_nothing = make_shared<DroneDoNothing>(player);
   this->state_chase = make_shared<DroneChase>(player);
}

void DroneDoNothing::doNothing(double delta_time, float d) {
   auto drone = static_pointer_cast<Drone>(this->enemy)->physics_component->getBody();
   drone->applyDamping(delta_time);
}

float DroneChase::DIRECT_CHASE_RADIUS = 100.0f;

void DroneChase::chase(double delta_time, float d) {

   // btVector3 target;
   // if (d < DroneChase::DIRECT_CHASE_RADIUS) {
   //    // close enough to the player, just chase them directly
   //    target = this->player->getTransform()->getOrigin();
   // } else {
   //    // navigate along the navmap
   //    auto nav_map = static_pointer_cast<Drone>(this->enemy)->getNavMap();
   //    target = this->brain->getNavTarget(nav_map);
   // }
   btVector3 target = this->player->getTransform()->getOrigin();

   this->moveToTarget(target, delta_time);

}

void DroneChase::moveToTarget(const btVector3 &target, const double delta_time) {
   btVector3 e = this->enemy->getTransform()->getOrigin();
   btVector3 f = 400.0f * (target - e).normalize();

   auto drone = static_pointer_cast<Drone>(this->enemy)->physics_component->getBody();
   drone->applyCentralForce(f);
}
