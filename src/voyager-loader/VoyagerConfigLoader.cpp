#include "include/VoyagerConfigLoader.h"

#include <iostream>
#include <string>

#include <rapidjson/document.h>

#include <voyager-utils/include/Path.h>

using namespace rapidjson;
using namespace std;

shared_ptr<VoyagerConfig> VoyagerConfigLoader::load(string path) {

   string the_parent_path = parent_path(path);

   Document doc = loadDocument(path);
   shared_ptr<VoyagerConfig> result;

   if (doc.HasMember("parent")) {
      string parent = doc["parent"].GetString();
      parent = the_parent_path + parent;
      result = this->load(parent);
   } else {
      result = make_shared<VoyagerConfig>();
   }

   if (doc.HasMember("resource_dir")) {
      result->resource_dir = the_parent_path + doc["resource_dir"].GetString();
   }

   if (doc.HasMember("type")) {
      result->type = doc["type"].GetString() == "SERVER" ? SERVER : CLIENT;
   }

   return result;

}

void VoyagerConfigLoader::store(shared_ptr<VoyagerConfig> thing, string path) {

   // we never really need this since configs are only loaded

   cerr << "VoyagerConfigLoader::store is not implemented" << endl;
   throw "not implemented";

}
