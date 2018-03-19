#include "include/Scene.h"

#include <voyager-actors/include/ai/AiEngine.h>

using namespace std;

shared_ptr<Terrain> Scene::initTerrain(shared_ptr<Application> app, shared_ptr<Entity> terrain) {
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
   renderEngine->setTerrainTextureScale(terrainShape->getTextureScale());

   return terrainShape;
}

void Scene::apply(shared_ptr<Application> app) {

   // Init HeightMap
   shared_ptr<Entity> terrain = this->entities.at(0);
   shared_ptr<Terrain> terrain_shape = initTerrain(app, terrain);

   // Set skybox
   static_pointer_cast<RenderEngine>(app->getRenderEngine())->setSkybox(this->skybox);

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

   // Init Nav Map
   shared_ptr<NavMapEntity> nav_map_entity = make_shared<NavMapEntity>(player, terrain_shape);
   this->entities.push_back(static_pointer_cast<Entity>(nav_map_entity));
   shared_ptr<NavMapRenderable> nav_map_renderable = make_shared<NavMapRenderable>(nav_map_entity->getNavMap());
   nav_map_entity->add(nav_map_renderable);
   this->components.push_back(nav_map_renderable);
   shared_ptr<AiEngine> ai = static_pointer_cast<AiEngine>(app->getAiEngine());
   ai->setNavMapEntity(nav_map_entity);

   // Transfer entities to the app
   for (int i = 0; i < this->entities.size(); ++i) {
      shared_ptr<Entity> entity = this->entities.at(i);
      app->getThings()[entity->getId()] = entity;
   }

   // transfer components to the app and assign them to engines
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
