#pragma once
#ifndef _SCENELOADER_H
#define _SCENELOADER_H

#include <rapidjson/document.h>

#include <voyager-core/include/All.h>

#include <voyager-utils/include/Terrain.h>
#include <voyager-utils/include/Skybox.h>

#include <voyager-render/include/Renderable.h>

#include <voyager-physics/include/PhysicsComponent.h>
#include <voyager-actors/include/enemies/Spawner.h>

#include <bullet/src/btBulletDynamicsCommon.h>

#include "Scene.h"
#include "JsonLoader.h"

using namespace std;
using namespace rapidjson;

class SceneLoader : public JsonLoader<Scene> {

public:

   SceneLoader(std::string resource_dir);

   virtual std::shared_ptr<Scene> load(std::string path);

   virtual void store(std::shared_ptr<Scene> thing, std::string path);

private:

   std::string resource_dir;

   void parse_terrain(std::shared_ptr<Scene> scene, rapidjson::Value& terrain);
   void parse_skybox(std::shared_ptr<Scene> scene, rapidjson::Value& skybox);
   void parse_meshes(std::shared_ptr<Scene> scene, rapidjson::Value& meshes);
   void parse_ubers(std::shared_ptr<Scene> scene, rapidjson::Value& ubers);
   void parse_entities(std::shared_ptr<Scene> scene, rapidjson::Value& entities);
   std::shared_ptr<Entity> parse_entity(std::shared_ptr<Scene> scene, rapidjson::Value& json, int tabs = 0);
   void parse_components(std::shared_ptr<Scene> scene, std::shared_ptr<Entity> entity,
      rapidjson::Value& components);

   std::shared_ptr<Component> parse_renderable(std::shared_ptr<Scene> scene, rapidjson::Value& component);
   std::shared_ptr<Component> parse_particles(shared_ptr<Scene> scene, rapidjson::Value& component);
   std::shared_ptr<Component> parse_playerComponent(shared_ptr<Entity> entity, std::shared_ptr<PhysicsComponent> physicsComponent, std::shared_ptr<Scene> scene, rapidjson::Value& component);
   std::shared_ptr<Component> parse_shipComponent(shared_ptr<Entity> entity, std::shared_ptr<PhysicsComponent> physicsComponent, std::shared_ptr<Scene> scene, rapidjson::Value& component);
   std::shared_ptr<PhysicsComponent> parse_physicsComponent(shared_ptr<Entity> entity, std::shared_ptr<Scene> scene, rapidjson::Value& component);
   std::shared_ptr<HelmComponent> parse_helmComponent(shared_ptr<Entity> entity, std::shared_ptr<Scene> scene, rapidjson::Value& component);
   std::shared_ptr<TurretComponent> parse_turretComponent(shared_ptr<Entity> entity, std::shared_ptr<Scene> scene, rapidjson::Value& component);
   std::shared_ptr<Component> parse_spawner(std::shared_ptr<Entity> entity, std::shared_ptr<Scene> scene, rapidjson::Value &component);
   void parse_transform(shared_ptr<Scene> scene, shared_ptr<Entity> entity, Value& transform);

};

#endif
