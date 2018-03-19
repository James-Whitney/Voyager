#include "../include/ai/NavMap.h"

#include <math.h>
#include <queue>
#include <set>
#include <stdio.h>

using namespace glm;
using namespace std;

NavMap::NavMap(ent_ptr_t player, terrain_ptr_t terrain, int res, float shift) :
   player(player), terrain(terrain)
{
   cout << "---< Initializing NavMap >---------------------------------" << endl;
   vert_grid_t grid = terrain->getVertices();

   // loop through the vertices, building a grid and adding waypoints to the map
   cout << "   * build the grid of Waypoints" << endl;
   for (int x = 0; x < grid.size(); x += res) {
      vector<wpt_ptr_t> waypoint_col;
      for (int y = 0; y < grid.at(x).size(); y += res) {

         // find a point for the nav map
         vertex pt = grid.at(x).at(y);
         vec3 nav_pt = pt.pos + shift * pt.normal;

         // make a waypoint and add it to the map
         wpt_ptr_t waypoint = make_shared<Waypoint>(nav_pt);
         this->addWaypoint(waypoint);
         waypoint_col.push_back(waypoint);
      }
      this->waypoint_grid.push_back(waypoint_col);
   }

   // now, iterate through the grid and link waypoints with edges
   cout << "   * link them together" << endl;
   for (int x = 0; x < waypoint_grid.size(); ++x) {
      int x_size = waypoint_grid.size();
      for (int y = 0; y < waypoint_grid.at(x).size(); ++y) {
         int y_size = waypoint_grid.at(x).size();

         // right
         if (x + 1 < x_size) {
            this->makeEdge(
               this->waypoint_grid.at(x).at(y),
               this->waypoint_grid.at(x + 1).at(y)
            );
         }

         // up
         if (y + 1 < y_size) {
            this->makeEdge(
               this->waypoint_grid.at(x).at(y),
               this->waypoint_grid.at(x).at(y + 1)
            );
         }

         // right, up
         if (x + 1 < x_size && y + 1 < y_size) {
            this->makeEdge(
               this->waypoint_grid.at(x).at(y),
               this->waypoint_grid.at(x + 1).at(y + 1)
            );
         }

         // left, up
         if (x - 1 > 0 && y + 1 < y_size) {
            this->makeEdge(
               this->waypoint_grid.at(x).at(y),
               this->waypoint_grid.at(x - 1).at(y + 1)
            );
         }

      }
   }

   cout << "-----------------------------------------------------------" << endl;
}

void NavMap::addWaypoint(wpt_ptr_t wpt) {
   this->insert(pair<int, wpt_list_t>(wpt->getId(), wpt_list_t()));
}

void NavMap::makeEdge(wpt_ptr_t start, wpt_ptr_t end) {
   assert(start != nullptr);
   assert(end != nullptr);

   // bidirectional
   this->addEdge(start, end);
   this->addEdge(end, start);
}

void NavMap::addEdge(wpt_ptr_t start, wpt_ptr_t end) {

   auto start_iter = this->find(start->getId());
   assert((start_iter != this->end()));

   auto end_iter = this->find(end->getId());
   assert((end_iter != this->end()));

   start_iter->second.push_back(end);
}

// A* search
wpt_list_t NavMap::navigate(wpt_ptr_t start, wpt_ptr_t end) {
   // TODO: implement
}

// Waypoint /////////////////////////////////////////////////////////////////////////////
static long nextId = 1;

Waypoint::Waypoint(vec3 location) : id(nextId++), location(location) {}

float Waypoint::distanceTo(wpt_ptr_t other) const {
   return sqrtf(
      powf(other->location.x - this->location.x, 2.0f) +
      powf(other->location.y - this->location.y, 2.0f) +
      powf(other->location.z - this->location.z, 2.0f)
   );
}
