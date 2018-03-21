#include "include/TurretComponent.h"

void TurretComponent::init() {

}

void TurretComponent::update(double delta_time) {
   if (active) {
      logicUpdate(delta_time);
      cameraUpdate(cameraHeight);
   }
}

void TurretComponent::fireBullet() {
   if (id == 0) {
      //Create Entity
      shared_ptr<Entity> entity = make_shared<Entity>();
      shared_ptr<btTransform> btTrans = make_shared<btTransform>();

      btTrans->setIdentity();
      btTrans->setOrigin(getEntity()->getTransform()->getOrigin() + btVector3( 0.0, 3.0, 0.0));
      btTrans->setRotation(ship->getEntity()->getTransform()->getRotation());
      entity->setTransform(btTrans);
      entity->setHealth(10.0);
      ////////RENDER
      shared_ptr<Renderable> renderComponent = make_shared<Renderable>();
      renderComponent->setMesh(app->getSceneMesh()->meshes[3]);
      renderComponent->setUber(app->getSceneMesh()->ubers[1]);
      renderComponent->setCullStatus(false);
      renderComponent->init();
      app->getRenderEngine()->registerComponent(std::static_pointer_cast<Component>(renderComponent));
      entity->add(std::static_pointer_cast<Component>(renderComponent));
      ////////PHYSICS
      shared_ptr<PhysicsComponent> physicsComponent = make_shared<PhysicsComponent>();
      btCollisionShape* collisionShape = new btSphereShape(0.05);
      btScalar mass(10.0);
      btVector3 position = getEntity()->getTransform()->getOrigin() + btVector3( 0.0, 3.0, 0.0);
      shared_ptr<btVector3> scale = make_shared<btVector3>(btVector3(0.1, 0.1, 0.1));
      entity->setScale(scale);
      btScalar friction = btScalar(0.1);
      // btVector3 axis = btVector3();
      // btScalar rotation = btScalar();
      btQuaternion rotationQuat = ship->getEntity()->getTransform()->getRotation();
      btVector3 velocity = btVector3(0.0, 0.0, 0.0);
      physicsComponent->initRigidBody(1, entity, collisionShape, mass, position, rotationQuat, velocity, friction);
      app->getPhysicsEngine()->registerComponent(std::static_pointer_cast<Component>(physicsComponent));
      btVector3 lookAt = glmToBullet(camera->getLookAt(false));
      physicsComponent->getBody()->applyCentralForce(lookAt * 100000);
      entity->add(std::static_pointer_cast<Component>(physicsComponent));

      ////////BULLET
      shared_ptr<BulletComponent> bulletComponent = make_shared<BulletComponent>();
      bulletComponent->setApp(app);
      app->getActorEngine()->registerComponent(std::static_pointer_cast<Component>(bulletComponent));
      entity->add(std::static_pointer_cast<Component>(bulletComponent));
      ////////

      app->getThings()[entity->getId()] = entity;
      //printf("Bullet Made %d\n", entity->getId());
   }
   else if (id == 1) {
      //Create Entity
      shared_ptr<Entity> entity = make_shared<Entity>();
      shared_ptr<btTransform> btTrans = make_shared<btTransform>();

      btTrans->setIdentity();
      btTrans->setOrigin(getEntity()->getTransform()->getOrigin() + btVector3( 0.0, 3.0, 0.0));
      btTrans->setRotation(ship->getEntity()->getTransform()->getRotation());
      entity->setTransform(btTrans);
      entity->setHealth(100.0);
      ////////RENDER
      shared_ptr<Renderable> renderComponent = make_shared<Renderable>();
      renderComponent->setMesh(app->getSceneMesh()->meshes[3]);
      renderComponent->setUber(app->getSceneMesh()->ubers[1]);
      renderComponent->setCullStatus(false);
      renderComponent->init();
      app->getRenderEngine()->registerComponent(std::static_pointer_cast<Component>(renderComponent));
      entity->add(std::static_pointer_cast<Component>(renderComponent));
      ////////PHYSICS
      shared_ptr<PhysicsComponent> physicsComponent = make_shared<PhysicsComponent>();
      btCollisionShape* collisionShape = new btSphereShape(0.1);
      btScalar mass(10000.0);
      btVector3 position = getEntity()->getTransform()->getOrigin() + btVector3( 0.0, 3.0, 0.0);
      shared_ptr<btVector3> scale = make_shared<btVector3>(btVector3(0.2, 0.2, 0.2));
      entity->setScale(scale);
      btScalar friction = btScalar(0.1);
      // btVector3 axis = btVector3();
      // btScalar rotation = btScalar();
      btQuaternion rotationQuat = ship->getEntity()->getTransform()->getRotation();
      btVector3 velocity = btVector3(0.0, 0.0, 0.0);
      physicsComponent->initRigidBody(1, entity, collisionShape, mass, position, rotationQuat, velocity, friction);
      app->getPhysicsEngine()->registerComponent(std::static_pointer_cast<Component>(physicsComponent));
      btVector3 lookAt = glmToBullet(camera->getLookAt(false));
      physicsComponent->getBody()->applyCentralForce(lookAt * 100000000);
      entity->add(std::static_pointer_cast<Component>(physicsComponent));

      ////////BULLET
      shared_ptr<BulletComponent> bulletComponent = make_shared<BulletComponent>();
      bulletComponent->setApp(app);
      app->getActorEngine()->registerComponent(std::static_pointer_cast<Component>(bulletComponent));
      entity->add(std::static_pointer_cast<Component>(bulletComponent));
      ////////

      app->getThings()[entity->getId()] = entity;
      //printf("Bullet Made %d\n", entity->getId());
   }
}

void TurretComponent::logicUpdate(double delta_time) {
   fire_recoil -= delta_time;
   if (glfwGetKey(window->getHandle(), GLFW_KEY_SPACE ) == GLFW_PRESS) {
      if (fire_recoil < 0.0) {
         
         fire_recoil = ( id == 0 ? 5 : 100 );
         fireBullet();
      }
   }
}
