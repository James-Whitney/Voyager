#include "include/SceneLoader.h"

#include <iostream>
#include <sstream>

#include <voyager-render/include/Renderable.h>

#define _SCENELOADER_LOG 1 // set to 1 to log as the scene is loading

using namespace glm;
using namespace rapidjson;
using namespace std;

void log(string msg) {
#if _SCENELOADER_LOG
   cout << msg << endl;
#endif
}

SceneLoader::SceneLoader(string resource_dir) : resource_dir(resource_dir) {}

shared_ptr<Scene> SceneLoader::load(string path) {
   log("-<Scene>-------[ " + path + " ]---------------");
   Document doc = loadDocument(path);
   shared_ptr<Scene> scene = make_shared<Scene>();

   if (doc.HasMember("terrain")) {
      this->parse_terrain(scene, doc["terrain"]);
   }

   if (doc.HasMember("shapes") && doc["shapes"].IsArray()) {
      this->parse_shapes(scene, doc["shapes"]);
   }

   if (doc.HasMember("ubers") && doc["ubers"].IsArray()) {
      this->parse_ubers(scene, doc["ubers"]);
   }

   if (doc.HasMember("entities") && doc["entities"].IsArray()) {
      this->parse_entities(scene, doc["entities"]);
   }

   log("-<Scene>----------------------------------------------------------------------");
   return scene;
}

void SceneLoader::store(shared_ptr<Scene> thing, string path) {
   throw "not implemented";
}

void SceneLoader::parse_terrain(shared_ptr<Scene> scene, Value& terrain) {
   cout << this->resource_dir + terrain["heightmap"].GetString() << endl;
   cout << terrain["height"].GetFloat() << endl;
   cout << terrain["spacing"].GetFloat() << endl;
   shared_ptr<Terrain> terrain_shape = make_shared<Terrain>();

   string heightmap_path = this->resource_dir + terrain["heightmap"].GetString();
   float max_height = terrain["height"].GetFloat();
   float vertex_spacing = terrain["spacing"].GetFloat();
   terrain_shape->createShape(heightmap_path, max_height, vertex_spacing);
   terrain_shape->measure();
   scene->shapes.push_back(terrain_shape);
}

void SceneLoader::parse_shapes(shared_ptr<Scene> scene, Value& shapes) {
   log("Shapes:");
   vector<tinyobj::shape_t> obj_shape;
   vector<tinyobj::material_t> obj_mat;
   string err;

   for (SizeType i = 0; i < shapes.Size(); ++i) {
      string shape_path = this->resource_dir + shapes[i]["path"].GetString();
      stringstream ss;
      ss << "\t" << i << ": " << shape_path;
      log(ss.str());

      if (!tinyobj::LoadObj(obj_shape, obj_mat, err, shape_path.c_str())) {
         cerr << "Failed to load " << shape_path << ": " << err << endl;
      } else {
         shared_ptr<Shape> shape = make_shared<Shape>();
         shape->createShape(obj_shape[0]);
         shape->measure();
         scene->shapes.push_back(shape);
      }
   }
}

void SceneLoader::parse_ubers(shared_ptr<Scene> scene, Value& ubers) {
   log("Ubers:");

   for (SizeType i = 0; i < ubers.Size(); ++i) {
      string type = ubers[i]["type"].GetString();
      stringstream ss;
      ss << "\t" << i << ": " << type;
      log(ss.str());

      shared_ptr<Uber> uber;
      if (type == "SIMPLE") {
         Value& ambient = ubers[i]["ambient"];
         Value& diffuse = ubers[i]["diffuse"];
         uber = make_shared<SimpleUber>(
            vec3(ambient[0].GetFloat(), ambient[1].GetFloat(), ambient[2].GetFloat()),
            vec3(diffuse[0].GetFloat(), diffuse[1].GetFloat(), diffuse[2].GetFloat())
         );
      } else if (type == "COOK_TORRANCE") {
         Value& ambient = ubers[i]["ambient"];
         uber = make_shared<CookTorranceUber>(
            ubers[i]["opacity"].GetFloat(),
            vec3(ambient[0].GetFloat(), ambient[1].GetFloat(), ambient[2].GetFloat()),
            ubers[i]["roughness"].GetFloat(),
            ubers[i]["f0"].GetFloat(),
            ubers[i]["k"].GetFloat()
         );
      } else {
         cerr << "Unknown shape type: " << type << endl;
         continue;
      }

      scene->ubers.push_back(uber);
   }
}

void SceneLoader::parse_entities(shared_ptr<Scene> scene, Value& entities) {
   log("Entities And Components:");
   for (SizeType i = 0; i < entities.Size(); ++i) {
      stringstream ss;
      ss << "\t" << i << ": " << entities[i]["name"].GetString();
      log(ss.str());

      shared_ptr<Entity> entity = make_shared<Entity>();

      Value& scale = entities[i]["transform"]["scale"];
      Value& position = entities[i]["transform"]["position"];
      Value& rotation = entities[i]["transform"]["rotation"];
      entity->setTransform(make_shared<Transform>(
         vec3(position[0].GetFloat(), position[1].GetFloat(), position[2].GetFloat()),
         vec3(scale[0].GetFloat(), scale[1].GetFloat(), scale[2].GetFloat()),
         rotation[0].GetFloat(), rotation[1].GetFloat(), rotation[2].GetFloat()
      ));

      if (entities[i].HasMember("components") && entities[i]["components"].IsArray()) {
         this->parse_components(scene, entity, entities[i]["components"]);
      }

      scene->entities.push_back(entity);
      for (int j = 0; j < entity->numComponents(); ++j) {
         scene->components.push_back(entity->componentAt(j));
      }
   }
}

void SceneLoader::parse_components(shared_ptr<Scene> scene, shared_ptr<Entity> entity,
      Value& components) {
   for (SizeType i = 0; i < components.Size(); ++i) {
      string type = components[i]["type"].GetString();
      stringstream ss;
      ss << "\t\t" << i << ": " << type;
      log(ss.str());

      if (type == "RENDER") {
         entity->add(this->parse_renderable(scene, components[i]));
      } else {
         cerr << "Unknown component type: " << type << endl;
         continue;
      }
   }
}

shared_ptr<Component> SceneLoader::parse_renderable(shared_ptr<Scene> scene, Value& component) {
   shared_ptr<Renderable> renderable = make_shared<Renderable>();

   int index = component["shape"].GetInt();
   renderable->setShape(scene->shapes[index]);

   index = component["uber"].GetInt();
   renderable->setUber(scene->ubers[index]);

   return static_pointer_cast<Component>(renderable);
}
