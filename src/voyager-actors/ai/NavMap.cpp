#include "../include/ai/NavMap.h"

#include <math.h>
#include <queue>
#include <set>
#include <stdio.h>

using namespace glm;
using namespace std;

NavMap::NavMap(ent_ptr_t player, terrain_ptr_t terrain, btVector3 &terrain_origin, int res, float shift) :
   player(player), terrain(terrain)
{
   vert_grid_t grid = terrain->getVertices();
   vec3 terr_origin = vec3(
      terrain_origin.getX(), terrain_origin.getY(), terrain_origin.getZ()
   );

   // loop through the vertices, building a grid and adding waypoints to the map
   for (int x = 0; x < grid.size(); x += res) {
      vector<wpt_ptr_t> waypoint_col;
      for (int y = 0; y < grid.at(x).size(); y += res) {

         // find a point for the nav map
         vertex pt = grid.at(x).at(y);
         // vec3 nav_pt = 2.0f * ((pt.pos + terr_origin) + shift * pt.normal);
         vec3 nav_pt = 2.0f * pt.pos + terr_origin;
         nav_pt += shift * pt.normal;

         // make a waypoint and add it to the map
         wpt_ptr_t waypoint = make_shared<Waypoint>(nav_pt);
         this->addWaypoint(waypoint);
         waypoint_col.push_back(waypoint);
      }
      this->waypoint_grid.push_back(waypoint_col);
   }

   // now, iterate through the grid and link waypoints with edges
   for (int x = 0; x < waypoint_grid.size(); ++x) {
      int x_size = waypoint_grid.size();
      for (int y = 0; y < waypoint_grid.at(x).size(); ++y) {
         wpt_ptr_t wpt = this->waypoint_grid.at(x).at(y);
         int y_size = waypoint_grid.at(x).size();

         // right
         if (x + 1 < x_size) {
            this->makeEdge(wpt, this->waypoint_grid.at(x + 1).at(y));
         }

         // up
         if (y + 1 < y_size) {
            this->makeEdge(wpt, this->waypoint_grid.at(x).at(y + 1));
         }

         // right, up
         if (x + 1 < x_size && y + 1 < y_size) {
            this->makeEdge(wpt, this->waypoint_grid.at(x + 1).at(y + 1));
         }

         // left, up
         if (x - 1 > 0 && y + 1 < y_size) {
            this->makeEdge(wpt, this->waypoint_grid.at(x - 1).at(y + 1));
         }

         // add it to the id map for quick lookup
         this->id_to_waypoint[wpt->getId()] = wpt;
      }
   }

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

score_map_t NavMap::getInitialScoreMap(wpt_ptr_t start, float start_val) {
   score_map_t score_map;
   for (int x = 0; x < this->waypoint_grid.size(); ++x) {
      for (int y = 0; y < this->waypoint_grid.at(x).size(); ++y) {
         wpt_ptr_t wpt = this->waypoint_grid.at(x).at(y);
         score_map[wpt->getId()] = numeric_limits<float>::max();
      }
   }
   score_map[start->getId()] = start_val;
   return score_map;
}

// A* search
wpt_list_t NavMap::navigate(wpt_ptr_t start, wpt_ptr_t end, int max_depth) {
   // initialize variables
   // set<Waypoint> closed_set;
   // set<Waypoint> open_set;
   // priority_queue<Waypoint, vector<Waypoint> > open_queue;
   // open_set.insert(*start);
   // open_queue.push(*start);
   // unordered_map<int, int> came_from; // key came from value
   // score_map_t g_score = this->getInitialScoreMap(start, 0);
   // score_map_t f_score = this->getInitialScoreMap(start, this->heuristic(start, end));

   // // iterate
   // int depth = 0;
   // while (open_set.size() > 0) {
   //    ++depth;

   //    // did we make it?
   //    Waypoint current = open_queue.top();
   //    if (current.getId() == end->getId() || (max_depth >= 0 && depth > max_depth)) {
   //       return this->reconstructPath(came_from, current.getId());
   //    }

   //    // move to the next one
   //    open_set.erase(current);
   //    open_queue.pop();
   //    closed_set.insert(current);

   //    // setup the neighbors
   //    wpt_list_t neighbors = this->find(current.getId())->second;
   //    for (wpt_ptr_t n : neighbors) {

   //       // already evaluated?
   //       if (closed_set.find(*n) != closed_set.end())
   //          continue;

   //       // new node?
   //       if (open_set.find(*n) == open_set.end()) {
   //          open_set.insert(*n);
   //          open_queue.push(*n);
   //       }

   //       // do we have a better path?
   //       float tentative_g = g_score[current.getId()] + current.distanceTo(n);
   //       if (tentative_g >= g_score[n->getId()]) {
   //          // no...
   //          continue;
   //       }

   //       // yes!
   //       came_from[n->getId()] = current.getId();
   //       g_score[n->getId()] = tentative_g;
   //       f_score[n->getId()] = g_score[n->getId()] + this->heuristic(n, end);

   //    }
   // }
}

wpt_list_t NavMap::reconstructPath(unordered_map<int, int> came_from, int current) {
   wpt_list_t result = {this->id_to_waypoint[current]};
   while (came_from.find(current) != came_from.end()) {
      current = came_from[current];
      result.push_back(this->id_to_waypoint[current]);
   }
   return result;
}

float NavMap::heuristic(wpt_ptr_t start, wpt_ptr_t end) {
   // for now, simply the distance between them
   assert(start != nullptr);
   assert(end != nullptr);
   return end->distanceTo(start);
}

wpt_ptr_t NavMap::getNearestWaypoint(const btVector3 &point) {
   // TODO:
   // currently brute forcing this...
   // if performance sucks, this is a spot that could be sped up
   wpt_ptr_t result = nullptr;
   float record = std::numeric_limits<float>::max();
   for (int x = 0; x < this->waypoint_grid.size(); ++x) {
      for (int y = 0; y < this->waypoint_grid.at(x).size(); ++y) {
         wpt_ptr_t wpt = this->waypoint_grid.at(x).at(y);
         float d = wpt->distanceTo(result);
         if (result == nullptr || d < record) {
            result = wpt;
            record = d;
         }
      }
   }
   assert(result != nullptr);
   return result;
}

// Waypoint /////////////////////////////////////////////////////////////////////////////
static long nextId = 1;

Waypoint::Waypoint(vec3 location) : id(nextId++), location(location) {}

float Waypoint::distanceTo(wpt_ptr_t other) const {
   if (other == nullptr) {
      return std::numeric_limits<float>::max();
   } else {
      return sqrtf(
         powf(other->location.x - this->location.x, 2.0f) +
         powf(other->location.y - this->location.y, 2.0f) +
         powf(other->location.z - this->location.z, 2.0f)
      );
   }
}

// WaypointCompare::WaypointCompare(score_map_t *f_score) : f_score(f_score) {}
// bool WaypointCompare::operator() (Waypoint a, Waypoint b) {
//   return true;
// }
