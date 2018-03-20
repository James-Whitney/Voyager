#include "include/Scene.h"

#include <voyager-actors/include/ai/AiEngine.h>
#include <voyager-actors/include/enemies/Drone.h>

#define SHOW_NAVMAP_WAYPOINTS 1

using namespace std;

shared_ptr<Terrain> Scene::initTerrain(shared_ptr<Application> app, shared_ptr<Entity> terrain) {

   this->terrain_entity = terrain;

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
   shared_ptr<NavMapEntity> nav_map_entity = make_shared<NavMapEntity>(ship, this->terrain_entity->getTransform(), terrain_shape);
   shared_ptr<NavMapRenderable> nav_map_renderable = make_shared<NavMapRenderable>(nav_map_entity->getNavMap());
   nav_map_entity->add(nav_map_renderable);
   this->entities.push_back(static_pointer_cast<Entity>(nav_map_entity));
   this->components.push_back(nav_map_renderable);
   shared_ptr<AiEngine> ai = static_pointer_cast<AiEngine>(app->getAiEngine());
   ai->setNavMapEntity(nav_map_entity);

#if SHOW_NAVMAP_WAYPOINTS
   // Nav Map markers
   vector<vector<wpt_ptr_t>> navGrid = nav_map_entity->getNavMap()->getWaypointGrid();
   for (int x = 0; x < navGrid.size(); ++x) {
      for (int y = 0; y < navGrid.at(x).size(); ++y) {
         shared_ptr<Entity> ent = this->make_waypoint_marker(navGrid.at(x).at(y));
         this->inject_entity(ent);
      }
   }
#endif

   // TEMPORARILY INJECT A DRONE HERE
   shared_ptr<btTransform> drone_trans = make_shared<btTransform>();
   drone_trans->setOrigin(btVector3(30, 0, 40));
   drone_trans->setRotation(btQuaternion(btVector3(0, 0, 1), 0));
   shared_ptr<Drone> drone = make_shared<Drone>(this->shared_from_this(), nav_map_entity->getNavMap(), drone_trans);
   drone->linkComponents();
   this->inject_entity(drone);

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
      } else if (dynamic_pointer_cast<BrainComponent>(component)) {
         app->getAiEngine()->registerComponent(component);
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

shared_ptr<Entity> Scene::make_waypoint_marker(wpt_ptr_t wpt) {
   shared_ptr<Entity> ent = make_shared<Entity>();

   // make the transform
   shared_ptr<btTransform> trans = make_shared<btTransform>();
   btVector3 pos = btVector3(
      wpt->getLocation().x, wpt->getLocation().y, wpt->getLocation().z
   );
   trans->setOrigin(pos);
   btQuaternion quat = btQuaternion(btVector3(0, 0, 1), 0);
   trans->setRotation(quat);
   ent->setTransform(trans);
   ent->setScale(make_shared<btVector3>(btVector3(1, 1, 1)));

   // make the render component
   shared_ptr<Renderable> rend = make_shared<Renderable>();
   rend->setMesh(this->meshes[1]);
   rend->setUber(this->ubers[5]);
   rend->setCullStatus(false);
   ent->add(static_pointer_cast<Component>(rend));

   return ent;
}

void Scene::inject_entity(shared_ptr<Entity> entity) {
   this->entities.push_back(entity);
   for (int i = 0; i < entity->numComponents(); ++i) {
      this->components.push_back(entity->componentAt(i));
   }
}
