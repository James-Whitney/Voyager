#pragma once
#ifndef _SCENELOADER_H
#define _SCENELOADER_H

#include <rapidjson/document.h>

#include <voyager-core/include/All.h>

#include "Scene.h"
#include "JsonLoader.h"

class SceneLoader : public JsonLoader<Scene> {

public:

   SceneLoader(std::string resource_dir);

   virtual std::shared_ptr<Scene> load(std::string path);

   virtual void store(std::shared_ptr<Scene> thing, std::string path);

private:

   std::string resource_dir;

   void parse_shapes(std::shared_ptr<Scene> scene, rapidjson::Value& shapes);
   void parse_ubers(std::shared_ptr<Scene> scene, rapidjson::Value& ubers);
   void parse_entities(std::shared_ptr<Scene> scene, rapidjson::Value& entities);
   void parse_components(std::shared_ptr<Scene> scene, std::shared_ptr<Entity> entity,
      rapidjson::Value& components);

   std::shared_ptr<Component> parse_renderable(std::shared_ptr<Scene> scene, rapidjson::Value& component);

};

#endif
