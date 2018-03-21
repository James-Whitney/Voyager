#include "include/Scene.h"



using namespace std;

void Scene::initTerrain(shared_ptr<Application> app, shared_ptr<Entity> terrain) {
   shared_ptr<Renderable> terrainRenderable = static_pointer_cast<Renderable>(terrain->componentAt(0));
   shared_ptr<Terrain> terrainShape = static_pointer_cast<Terrain>(terrainRenderable->getMesh().at(0));

   shared_ptr<PhysicsComponent> physicsComponent = make_shared<PhysicsComponent>();

   int mapWidth = terrainShape->getMapWidth();
   int mapLength = terrainShape->getMapLength();
   vector<float> heightData = terrainShape->getHeightData();
   btScalar heightScale = terrainShape->getHeightScale();
   btScalar minHeight = terrainShape->getMinHeight();
   btScalar maxHeight = terrainShape->getMaxHeight();
   btScalar vertexScale = terrainShape->getVertexSpacing();

   heightScale = 1.0;
   // maxHeight = 100.0;

   printf("mapWidth: %d\n", mapWidth);
   printf("mapLength: %d\n", mapLength);
   printf("heightScale: %f\n", heightScale);
   printf("minHeight: %f\n", minHeight);
   printf("maxHeight: %f\n", maxHeight);
   printf("vertexScale: %f\n", vertexScale);

   //                                                                                                                vertexscale, heightScale
   btHeightfieldTerrainShape *collisionShape = new btHeightfieldTerrainShape( mapWidth, mapLength, (void *)(&heightData[0]), vertexScale, minHeight, maxHeight, 1, PHY_FLOAT, false);
   // btCollisionShape *collisionShape = new btHeightfieldTerrainShape( mapWidth, mapLength, (void *)(&heightData[0]), vertexScale, minHeight, maxHeight, 1, PHY_FLOAT, false);
   printf("Collision Shape made\n");
   btVector3 position = btVector3(mapWidth/2.0, 0, mapLength/2.0);
   physicsComponent->initHeightMap( terrain, position, terrain->getTransform()->getRotation(), collisionShape );
   printf("initHeightMap\n");

   terrain->add(physicsComponent);
   app->getPhysicsEngine()->registerComponent(physicsComponent);

   // Set terrain texture and normal map in render engine
   shared_ptr<RenderEngine> renderEngine = static_pointer_cast<RenderEngine>(app->getRenderEngine());
   renderEngine->setTerrainTexture(terrainShape->getTexture());
   renderEngine->setTerrainNormalMap(terrainShape->getNormalMap());
   renderEngine->setTerrainTextureScale(terrainShape->getTextureScale());

   btVector3 btAabbMin, btAabbMax;
   physicsComponent->getBody()->getAabb(btAabbMin, btAabbMax);
   shared_ptr<DebugBox> debugBox = make_shared<DebugBox>(btAabbMin, btAabbMax);
   debugBox->setShape(physicsComponent->getBody()->getCollisionShape());
   debugBox->setEntity(terrain);
   debugBoxes.push_back(debugBox);
   printf("initTerrain\n");
}

void Scene::apply(shared_ptr<Application> app) {

   for (int i = 0; i < this->entities.size(); ++i) {
      shared_ptr<Entity> entity = this->entities.at(i);
      app->getThings()[entity->getId()] = entity;
   }

   // Init HeightMap
   shared_ptr<Entity> terrain = this->entities.at(0);
   initTerrain(app, terrain);

   shared_ptr<RenderEngine> renderEngine = static_pointer_cast<RenderEngine>(app->getRenderEngine());

   // Set skybox
   renderEngine->setSkybox(this->skybox);

   // Set debug boxes
   renderEngine->setDebugBoxes(this->debugBoxes);

   // Init ship
   shared_ptr<Entity> ship = this->entities.at(1);
   shared_ptr<ShipComponent> shipComponent =
      static_pointer_cast<ShipComponent>(ship->componentAt(ship->numComponents()-1));
   static_pointer_cast<PhysicsEngine>(app->getPhysicsEngine())->setShip(shipComponent);
   //shipComponent->getPhysics()->getBody()->setGravity(btVector3(0, 1, 0));

   // Init player
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
            static_pointer_cast<StationComponent>(component)->setShip(shipComponent);            
         }
         if (dynamic_pointer_cast<HelmComponent>(component)) {
            playerComponent->setHelm(static_pointer_cast<StationComponent>(component));
            static_pointer_cast<RenderEngine>(app->getRenderEngine())->setHelm(static_pointer_cast<HelmComponent>(component));
         }
         else if (dynamic_pointer_cast<TurretComponent>(component)) {
            playerComponent->setTurret(static_pointer_cast<StationComponent>(component), static_pointer_cast<TurretComponent>(component)->getTurretID());
            static_pointer_cast<TurretComponent>(component)->setApp(app);
         }
         app->getActorEngine()->registerComponent(component);
      }
   }
   app->setSceneMesh(this->shared_from_this());
}

void Scene::dump() {
   cout << "<Scene>" << endl;
   cout << this->meshes.size() << " meshes" << endl;
   cout << this->ubers.size() << " ubers" << endl;
   cout << this->entities.size() << " entities" << endl;
   cout << this->components.size() << " components" << endl;
   cout << "</Scene>" << endl;
}
