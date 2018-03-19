#include "../include/ai/NavMapEntity.h"

#include <cassert>
#include <stdio.h>

using namespace std;

// NavMapEntity ///////////////////////////////////////////////////////////

NavMapEntity::NavMapEntity(ent_ptr_t player, shared_ptr<Terrain> terrain) {
   this->nav_map = make_shared<NavMap>(player, terrain, 50, 4);
}

// NavMapRenderable ///////////////////////////////////////////////////////

NavMapRenderable::NavMapRenderable(shared_ptr<NavMap> nav_map) :
   nav_map(nav_map)
{}

void NavMapRenderable::init() {
   if (!NAV_MAP_DO_RENDER) {
      return;
   }
}

void NavMapRenderable::renderableInit(string resource_dir) {
   if (!NAV_MAP_DO_RENDER) {
      return;
   }
}

void NavMapRenderable::draw(shared_ptr<Program> prog) {
   if (!NAV_MAP_DO_RENDER) {
      return;
   }
}
