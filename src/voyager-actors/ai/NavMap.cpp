#include "../include/ai/NavMap.h"

#include <math.h>
#include <queue>
#include <set>

using namespace glm;
using namespace std;

NavMap::NavMap(ent_ptr_t player, terrain_ptr_t terrain, int res, float shift) :
   player(player), terrain(terrain)
{
   vert_grid_t grid = terrain->getVertices();
   vector<vector<wpt_ptr_t>> waypoint_grid((grid.size() / res) + 1);

   // loop through the vertices, building a grid and adding waypoints to the map
   for (int x = 0; x < grid.size(); x += res) {
      vector<wpt_ptr_t> waypoint_col((grid.size() / res) + 1);
      for (int y = 0; y < grid.at(x).size(); y += res) {

         // find a point for the nav map
         vertex pt = grid.at(x).at(y);
         vec3 nav_pt = pt.pos + shift * pt.normal;

         // make a waypoint and add it to the map
         wpt_ptr_t waypoint = make_shared<Waypoint>(nav_pt);
         this->addWaypoint(waypoint);
         waypoint_col.push_back(waypoint);
      }
      waypoint_grid.push_back(waypoint_col);
   }

   // now, iterate through the grid and link waypoints with edges
   for (int x = 0; x < waypoint_grid.size(); ++x) {
      int x_size = waypoint_grid.size();
      for (int y = 0; y < waypoint_grid.at(x).size(); ++y) {
         int y_size = waypoint_grid.at(x).size();

         // right
         if (x + 1 < x_size) {
            this->makeEdge(
               waypoint_grid.at(x).at(y),
               waypoint_grid.at(x + 1).at(y)
            );
         }

         // up
         if (y + 1 < y_size) {
            this->makeEdge(
               waypoint_grid.at(x).at(y),
               waypoint_grid.at(x).at(y + 1)
            );
         }

         // right, up
         if (x + 1 < x_size && y + 1 < y_size) {
            this->makeEdge(
               waypoint_grid.at(x).at(y),
               waypoint_grid.at(x + 1).at(y + 1)
            );
         }

         // left, up
         if (x - 1 > 0 && y + 1 < y_size) {
            this->makeEdge(
               waypoint_grid.at(x).at(y),
               waypoint_grid.at(x - 1).at(y + 1)
            );
         }

      }
   }

}

void NavMap::addWaypoint(wpt_ptr_t wpt) {
   this->insert(pair<wpt_ptr_t, wpt_list_t>(wpt, wpt_list_t()));
   wpt->setNavMap(this->shared_from_this());
}

void NavMap::makeEdge(wpt_ptr_t start, wpt_ptr_t end) {
   // bidirectional
   this->addEdge(start, end);
   this->addEdge(end, start);
}

// float NavMap::cost(wpt_ptr_t wpt, wpt_ptr_t start) {
//    // TODO: make a better cost
//    return start->distanceTo(wpt);
// }

// float NavMap::heuristic(wpt_ptr_t wpt, wpt_ptr_t end) {
//    // TODO: make a better heuristic
//    return wpt->distanceTo(end);
// }

// A* search
wpt_list_t NavMap::navigate(wpt_ptr_t start, wpt_ptr_t end) {

   // auto open_list = priority_queue<wpt_ptr_t>();
   // auto closed_list = std::set<wpt_ptr_t>();
   // open_list.push(start);

   // while (false /* the destination has not been reached */) {
   //    wpt_ptr_t cur_wpt = open_list.top();
   //    open_list.pop();

   //    if (false /* this wpt is the destination */) {
   //       // finished
   //    } else {
   //       closed_list.insert(cur_wpt);
   //       wpt_list_t neighbors = this->find(cur_wpt)->second;
   //       for (wpt_ptr_t neighbor : neighbors) {

   //       }
   //    }
   // }
}

void NavMap::addEdge(wpt_ptr_t start, wpt_ptr_t end) {
   auto start_iter = this->find(start);
   assert((start_iter != this->end()));
   start_iter->second.push_back(end);
}

float Waypoint::distanceTo(wpt_ptr_t other) const {
   return sqrtf(
      powf(other->location.x - this->location.x, 2.0f) +
      powf(other->location.y - this->location.y, 2.0f) +
      powf(other->location.z - this->location.z, 2.0f)
   );
}

float Waypoint::distanceToPlayer() const {
   auto player = this->getNavMap()->getPlayer()->getTransform(true)->getOrigin();
   return sqrtf(
      powf(player.getX() - this->location.x, 2.0f) +
      powf(player.getY() - this->location.y, 2.0f) +
      powf(player.getZ() - this->location.z, 2.0f)
   );
}

bool Waypoint::operator < (Waypoint other) const {
   return this->distanceToPlayer() < other.distanceToPlayer();
}
