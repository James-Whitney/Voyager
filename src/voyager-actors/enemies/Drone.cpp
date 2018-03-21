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
   btSphereShape *shape = new btSphereShape(2.4f * props.box_size);

   // initialize the component
   phys->initRigidBody(world, ent, shape, props.mass, position, rotation, velocity, props.friction);
   phys->getBody()->setDamping(props.lin_damp, props.ang_damp);

   return phys;
}

Drone::Drone(shared_ptr<Scene> scene, shared_ptr<NavMap> nav_map, shared_ptr<btTransform> trans,
      shared_ptr<Application> app) :
   Entity(),
   nav_map(nav_map),
   app(app)
{
   this->setTeamMask(12345);

   // transform
   this->setTransform(trans);
   DronePhysicsProperties props;
   this->setScale(make_shared<btVector3>(props.box_size, props.box_size, props.box_size));

   // renderable
   this->renderable = make_shared<Renderable>();
   this->renderable->setMesh(scene->meshes[4]);
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

void Drone::update(double delta_time) {
   Entity::update(delta_time);
}

void Drone::kill() {
   for (int i = 0; i < this->numComponents(); ++i) {
      this->componentAt(i)->setRemoveFlag();
   }
   for (int i = 0; i < this->numComponents(); ++i) {
      this->removeComponentAt(i);
   }
   assert(this->app != nullptr);
   this->app->getThings().erase(this->getId());
}

DroneBrain::DroneBrain(shared_ptr<Entity> player) : Brain(player) {
   this->state_do_nothing = make_shared<DroneDoNothing>(player);
   this->state_chase = make_shared<DroneChase>(player);
}

void DroneBrain::run(double delta_time) {
   Brain::run(delta_time);

   auto collisionList = this->enemy->getCollideList();

   for (auto &collision: collisionList) {
     float enemyHealth = *(collision->getHealth());
     float myHealth = *(this->enemy->getHealth());
     if (this->enemy->getTeamMask() != collision->getTeamMask()) {
         this->enemy->setHealth(myHealth - enemyHealth);
         cout << "drone health is now " << *(this->enemy->getHealth()) << endl;
     }
   }

   if (*(this->enemy->getHealth()) <= 0) {
      static_pointer_cast<Drone>(this->enemy)->kill();
   }
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
   DronePhysicsProperties props;
   btVector3 f = props.move_force * (target - e).normalize();

   auto drone = static_pointer_cast<Drone>(this->enemy)->physics_component->getBody();
   drone->applyCentralForce(f);
}
