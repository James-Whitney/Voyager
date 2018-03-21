#include "include/SceneLoader.h"

#include <iostream>
#include <cstring>

#include <sstream>

#define _SCENELOADER_LOG 1 // set to 1 to log as the scene is loading

using namespace glm;
using namespace rapidjson;
using namespace std;

static void log(string msg) {
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

   if (doc.HasMember("skybox")) {
      this->parse_skybox(scene, doc["skybox"]);
   }

   if (doc.HasMember("meshes") && doc["meshes"].IsArray()) {
      this->parse_meshes(scene, doc["meshes"]);
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
   shared_ptr<Terrain> terrain_shape = make_shared<Terrain>();
   string heightmap_path = this->resource_dir + terrain["heightmap"].GetString();
   float max_height = terrain["height"].GetFloat();
   float vertex_spacing = terrain["spacing"].GetFloat();
   float texture_scale = terrain["textureScale"].GetFloat();

   terrain_shape->createShape(heightmap_path, max_height, vertex_spacing, texture_scale);
   terrain_shape->measure();

   string texture_path = this->resource_dir + terrain["texture"].GetString();
   terrain_shape->setTextureFilename(texture_path);

   string normal_map_path = this->resource_dir + terrain["normal_map"].GetString();
   terrain_shape->setNormalMapFilename(normal_map_path);

   vector<shared_ptr<Shape>> mesh;
   mesh.push_back(terrain_shape);
   scene->meshes.push_back(mesh);
}

void SceneLoader::parse_skybox(std::shared_ptr<Scene> scene, rapidjson::Value& skybox) {
   string top = this->resource_dir + skybox["top"].GetString();
   string bottom = this->resource_dir + skybox["bottom"].GetString();
   string front = this->resource_dir + skybox["front"].GetString();
   string back = this->resource_dir + skybox["back"].GetString();
   string left = this->resource_dir + skybox["left"].GetString();
   string right = this->resource_dir + skybox["right"].GetString();

   scene->skybox = make_shared<Skybox>(top, bottom, front, back, left, right);

   Value& fog = skybox["fog"];
   Value& fog_color = fog["color"];
   scene->skybox->fog.color = glm::vec3(fog_color[0].GetFloat(),
                                        fog_color[1].GetFloat(),
                                        fog_color[2].GetFloat());
   scene->skybox->fog.density = fog["density"].GetFloat();
   scene->skybox->fog.height = fog["height"].GetFloat();
}

void SceneLoader::parse_meshes(shared_ptr<Scene> scene, Value& meshes) {
   log("Meshes:");
   vector<tinyobj::shape_t> obj_shapes;
   vector<tinyobj::material_t> obj_mat;
   string err;

   for (SizeType i = 0; i < meshes.Size(); ++i) {
      string mesh_path = this->resource_dir + meshes[i]["path"].GetString();
      stringstream ss;
      ss << "\t" << i << ": " << mesh_path;
      log(ss.str());

      if (!tinyobj::LoadObj(obj_shapes, obj_mat, err, mesh_path.c_str())) {
         cerr << "Failed to load " << mesh_path << ": " << err << endl;
      } else {
         vector<shared_ptr<Shape>> mesh;

         for (tinyobj::shape_t obj_shape : obj_shapes) {
            shared_ptr<Shape> shape = make_shared<Shape>();
            shape->createShape(obj_shape);
            shape->measure();
            mesh.push_back(shape);
         }

         scene->meshes.push_back(mesh);
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
      } else if (type == "NORMAL") {
         uber = make_shared<NormalUber>();
      } else if (type == "TERRAIN_COOK_TORRANCE") {
         Value& ambient = ubers[i]["ambient"];
         uber = make_shared<TerrainCookTorranceUber>(
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

void SceneLoader::parse_transform(shared_ptr<Scene> scene, shared_ptr<Entity> entity, Value& transform) {
   shared_ptr<btTransform> btTrans = make_shared<btTransform>();
   Value& position = transform["position"];
   btVector3 pos = btVector3(position[0].GetFloat(),
                             position[1].GetFloat(),
                             position[2].GetFloat());
   Value& axis = transform["axis"];
   Value& rotation = transform["rotation"];
   btQuaternion btQuad = btQuaternion(btVector3(axis[0].GetFloat(),
                                                axis[1].GetFloat(),
                                                axis[2].GetFloat()),
                                                rotation.GetFloat());

   Value& scale_ = transform["scale"];
   shared_ptr<btVector3> scale = make_shared<btVector3>(btVector3(scale_[0].GetFloat(),
                                                                  scale_[1].GetFloat(),
                                                                  scale_[2].GetFloat()));
   entity->setScale(scale);

   btTrans->setIdentity();
   btTrans->setOrigin(pos);
   btTrans->setRotation(btQuad);
   entity->setTransform(btTrans);
}

void SceneLoader::parse_entities(shared_ptr<Scene> scene, Value& entities) {
   log("Entities And Components:");
   for (SizeType i = 0; i < entities.Size(); ++i) {
      this->parse_entity(scene, entities[i], 1);
   }
}

shared_ptr<Entity> SceneLoader::parse_entity(shared_ptr<Scene> scene, rapidjson::Value& json, int tabs) {
   stringstream ss;
   while (tabs-- > 0) ss << '\t';
   ss << "entity: " << json["name"].GetString();
   log(ss.str());

   shared_ptr<Entity> entity = make_shared<Entity>();

   // transform
   if (json.HasMember("transform")) {
      this->parse_transform(scene, entity, json["transform"]);
   }

   // components
   if (json.HasMember("components") && json["components"].IsArray()) {
      this->parse_components(scene, entity, json["components"]);
   }

   // add to scene
   scene->entities.push_back(entity);
   for (int i = 0; i < entity->numComponents(); ++i) {
      scene->components.push_back(entity->componentAt(i));
   }

   // any children?
   if (json.HasMember("children") && json["children"].IsArray()) {
      for (int i = 0; i < json["children"].GetArray().Size(); ++i) {
         shared_ptr<Entity> child = this->parse_entity(scene, json["children"][i], tabs + 1);
         entity->addChild(child);
         child->setParent(entity);
      }
   }

   return entity;
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
      }
      else if (type == "PARTICLES") {
         entity->add(this->parse_particles(scene, components[i]));
      }
      else if (type == "PHYSICS") {
         shared_ptr<PhysicsComponent> physicsComponent = this->parse_physicsComponent(entity, scene, components[i]);
         entity->add(static_pointer_cast<Component>(physicsComponent));
         if ( components[i].HasMember("player") ) {
            entity->setMask(PLAYER_MASK);
            entity->add(this->parse_playerComponent(entity, physicsComponent, scene, components[i]));
         }
         else if ( components[i].HasMember("ship") ) {
            entity->add(this->parse_shipComponent(entity, physicsComponent, scene, components[i]));
            entity->setMask(SHIP_MASK);
            entity->setHealth(1000.0);
         }
      }
      else if (type == "STATION") {
         string subType = components[i]["sub-type"].GetString();
         if (subType == "HELM") {
            entity->add(this->parse_helmComponent(entity, scene, components[i]));
            entity->setMask(HELM_MASK);
         }
         else if (subType == "TURRET") {
            entity->add(this->parse_turretComponent(entity, scene, components[i]));
            entity->setMask(TURRET_MASK);
         }
      }
      else {
         cerr << "Unknown component type: " << type << endl;
         continue;
      }
   }
}

shared_ptr<Component> SceneLoader::parse_renderable(shared_ptr<Scene> scene, Value& component) {
   shared_ptr<Renderable> renderable = make_shared<Renderable>();

   int index = component["mesh"].GetInt();
   renderable->setMesh(scene->meshes[index]);

   index = component["uber"].GetInt();
   renderable->setUber(scene->ubers[index]);

   renderable->setCullStatus(component["static"].GetBool());

   return static_pointer_cast<Component>(renderable);
}

shared_ptr<Component> SceneLoader::parse_particles(shared_ptr<Scene> scene, Value& component) {
   shared_ptr<ParticleSystem> particleSystem = make_shared<ParticleSystem>();

   string tex_path = this->resource_dir + component["texture"].GetString();
   int unit = component["unit"].GetInt();

   shared_ptr<Texture> particleTexture = make_shared<Texture>();

   particleTexture->setFilename(tex_path);
   particleTexture->setUnit(unit);
   particleSystem->setTexture(particleTexture);

   return static_pointer_cast<Component>(particleSystem);
}

shared_ptr<Component> SceneLoader::parse_playerComponent(   shared_ptr<Entity> entity,
                                                            shared_ptr<PhysicsComponent> physicsComponent,
                                                            shared_ptr<Scene> scene,
                                                            Value& component) {

   shared_ptr<PlayerComponent> playerComponent = make_shared<PlayerComponent>();
   physicsComponent->getBody()->setActivationState(DISABLE_DEACTIVATION);
   physicsComponent->getBody()->setAngularFactor(btVector3(0,1,0));
   playerComponent->setPhysics(physicsComponent);
   return static_pointer_cast<Component>(playerComponent);
}

shared_ptr<Component> SceneLoader::parse_shipComponent(  shared_ptr<Entity> entity,
                                                         shared_ptr<PhysicsComponent> physicsComponent,
                                                         shared_ptr<Scene> scene,
                                                         Value& component) {

   shared_ptr<ShipComponent> shipComponent = make_shared<ShipComponent>();
   physicsComponent->getBody()->setActivationState(DISABLE_DEACTIVATION);
   physicsComponent->getBody()->setAngularFactor(btVector3(0,1,0));
   shipComponent->setPhysics(physicsComponent);
   shipComponent->setSpeed(component["speed"].GetFloat());
   return static_pointer_cast<Component>(shipComponent);
}

shared_ptr<HelmComponent> SceneLoader::parse_helmComponent( shared_ptr<Entity> entity,
                                                            shared_ptr<Scene> scene,
                                                            Value& component) {

   shared_ptr<HelmComponent> helmComponent = make_shared<HelmComponent>();
   helmComponent->setCameraHeight(component["cameraHeight"].GetFloat());
   helmComponent->setTurnSpeed(component["turnSpeed"].GetFloat());
   helmComponent->setRiseSpeed(component["riseSpeed"].GetFloat());
   return helmComponent;
}

shared_ptr<TurretComponent> SceneLoader::parse_turretComponent(shared_ptr<Entity> entity,
                                                               shared_ptr<Scene> scene,
                                                               Value& component) {

   shared_ptr<TurretComponent> turretComponent = make_shared<TurretComponent>();

   turretComponent->setTurretID(component["ID"].GetInt());
   turretComponent->setCameraHeight(component["cameraHeight"].GetFloat());
   turretComponent->setBulletType(component["bullet-type"].GetInt());
   return turretComponent;
}


shared_ptr<PhysicsComponent> SceneLoader::parse_physicsComponent( shared_ptr<Entity> entity,
                                                                  shared_ptr<Scene> scene,
                                                                  Value& component) {

   shared_ptr<PhysicsComponent> physicsComponent = make_shared<PhysicsComponent>();
   bool ghost = false;
   Value& world_ = component["world"];
   int world = world_.GetInt();

   btScalar lin_damp = btScalar(0.0);
   btScalar ang_damp = btScalar(0.0);

   if (component.HasMember("damping") && component["damping"].IsArray()) {
      lin_damp = btScalar(component["damping"][0].GetFloat());
      ang_damp = btScalar(component["damping"][1].GetFloat());
   }

   Value& fric = component["friction"];
   btScalar friction = btScalar(fric.GetFloat());

   btCollisionShape* collisionShape;

   Value& collision = component["collisionShape"];

   if ( strncmp(collision["type"].GetString(), "box", 3) == 0) {
      Value& box = component["collisionShape"]["scale"];
      btVector3 boxVec = btVector3(box[0].GetFloat(), box[1].GetFloat(), box[2].GetFloat());
      collisionShape = new btBoxShape(boxVec);
   }
   else if (strncmp(collision["type"].GetString(), "capsule", 7) == 0) {
      btScalar radius = component["collisionShape"]["radius"].GetFloat();
      btScalar height = component["collisionShape"]["height"].GetFloat();
      collisionShape = new btCapsuleShape(radius, height);
   }
   else if (strncmp(collision["type"].GetString(), "sphere", 6) == 0) {
      btScalar radius = component["collisionShape"]["radius"].GetFloat();
      collisionShape = new btSphereShape(radius);
   }

   if (component.HasMember("ghost")){
      ghost = true;
   }

   btScalar mass(component["mass"].GetFloat());

   Value& pos = component["position"];
   btVector3 position = btVector3(pos[0].GetFloat(),
                                  pos[1].GetFloat(),
                                  pos[2].GetFloat());

   Value& scale_ = component["scale"];
   shared_ptr<btVector3> scale = make_shared<btVector3>(btVector3(scale_[0].GetFloat(), scale_[1].GetFloat(), scale_[2].GetFloat()));
   //scale = btVector3(scale_[0].GetFloat(), scale_[1].GetFloat(), scale_[2].GetFloat());

   /*btVector3 scale = btVector3(  scale_[0].GetFloat(),
                                 scale_[1].GetFloat(),
                                 scale_[2].GetFloat());*/
   entity->setScale(scale);

   Value& axis = component["axis"];
   Value& rotation = component["rotation"];
   btQuaternion btQuad = btQuaternion(btVector3(axis[0].GetFloat(),
                                                axis[1].GetFloat(),
                                                axis[2].GetFloat()),
                                                rotation.GetFloat());

   Value& vel = component["velocity"];
   btVector3 velocity = btVector3(vel[0].GetFloat(),
                                  vel[1].GetFloat(),
                                  vel[2].GetFloat());
   if (ghost) {

   }
   physicsComponent->initRigidBody(world, entity, collisionShape, mass, position, btQuad, velocity, friction);
   physicsComponent->getBody()->setDamping(lin_damp, ang_damp);
   return physicsComponent;
}
