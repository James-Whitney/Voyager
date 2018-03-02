#include "include/Scene.h"



using namespace std;

void Scene::initTerrain(shared_ptr<Application> app, shared_ptr<Entity> terrain) {
   shared_ptr<Renderable> terrainRenderable = static_pointer_cast<Renderable>(terrain->componentAt(0));
   shared_ptr<Terrain> terrainShape = static_pointer_cast<Terrain>(terrainRenderable->getMesh().at(0));

   shared_ptr<PhysicsComponent> physicsComponent = make_shared<PhysicsComponent>();
   
   int mapWidth = terrainShape->getMapWidth();
   int mapLength = terrainShape->getMapLength();
   vector<unsigned char> heightData = terrainShape->getHeightData();
   btScalar heightScale = terrainShape->getHeightScale();
   btScalar minHeight = terrainShape->getMinHeight();
   btScalar maxHeight = terrainShape->getMaxHeight();
   btScalar vertexScale = terrainShape->getVertexSpacing();

   physicsComponent->initHeightMap( terrain,
                                    btVector3(0, 0, 0),
                                    terrain->getTransform()->getRotation(),
                                    mapWidth,
                                    mapLength,
                                    heightData,
                                    heightScale,
                                    minHeight,
                                    maxHeight,
                                    vertexScale);

   terrain->add(physicsComponent);
   app->getPhysicsEngine()->registerComponent(physicsComponent);

   // Set terrain texture and normal map in render engine
   shared_ptr<RenderEngine> renderEngine = static_pointer_cast<RenderEngine>(app->getRenderEngine());
   renderEngine->setTerrainTexture(terrainShape->getTexture());
   renderEngine->setTerrainNormalMap(terrainShape->getNormalMap());
}

void Scene::apply(shared_ptr<Application> app) {

   for (int i = 0; i < this->entities.size(); ++i) {
      shared_ptr<Entity> entity = this->entities.at(i);
      app->getThings()[entity->getId()] = entity;
   }

   //init HeightMap
   shared_ptr<Entity> terrain = this->entities.at(0);
   initTerrain(app, terrain);

   //init ship
   shared_ptr<Entity> ship = this->entities.at(1);
   shared_ptr<ShipComponent> shipComponent = 
      static_pointer_cast<ShipComponent>(ship->componentAt(ship->numComponents()-1));
   static_pointer_cast<PhysicsEngine>(app->getPhysicsEngine())->setShip(shipComponent);
   //shipComponent->getPhysics()->getBody()->setGravity(btVector3(0, 1, 0));

   //init player
   shared_ptr<Entity> player = this->entities.at(2);
   shared_ptr<PlayerComponent> playerComponent = 
      static_pointer_cast<PlayerComponent>(player->componentAt(player->numComponents()-1));
   playerComponent->setCamera(static_pointer_cast<RenderEngine>(app->getRenderEngine())->getCamera());
   playerComponent->setShip(shipComponent);
   //playerComponent->getPhysics()->getBody()->setGravity(btVector3(0, -9.8, 0));

   for (int i = 0; i < this->components.size(); ++i) {
      shared_ptr<Component> component = this->components.at(i);

      if (dynamic_pointer_cast<Renderable>(component)) {
         app->getRenderEngine()->registerComponent(component);
      }
      else if (dynamic_pointer_cast<PhysicsComponent>(component)) {
         app->getPhysicsEngine()->registerComponent(component);
      }
      else if (dynamic_pointer_cast<ActorComponent>(component)) {
         static_pointer_cast<StationComponent>(component)->setWindow(app->getWindowManager());
         if (dynamic_pointer_cast<StationComponent>(component)) {
            static_pointer_cast<StationComponent>(component)->setCamera(static_pointer_cast<RenderEngine>(app->getRenderEngine())->getCamera());
         }
         if (dynamic_pointer_cast<HelmComponent>(component)) {
            playerComponent->setHelm(static_pointer_cast<StationComponent>(component));
            static_pointer_cast<RenderEngine>(app->getRenderEngine())->setHelm(static_pointer_cast<HelmComponent>(component));
            static_pointer_cast<HelmComponent>(component)->setShip(shipComponent);
         }
         else if (dynamic_pointer_cast<TurretComponent>(component)) {
            playerComponent->setTurret(static_pointer_cast<StationComponent>(component), static_pointer_cast<TurretComponent>(component)->getTurretID());
         }
         app->getActorEngine()->registerComponent(component);
      }
   }
}

void Scene::dump() {
   cout << "<Scene>" << endl;
   cout << this->meshes.size() << " meshes" << endl;
   cout << this->ubers.size() << " ubers" << endl;
   cout << this->entities.size() << " entities" << endl;
   cout << this->components.size() << " components" << endl;
   cout << "</Scene>" << endl;
}
