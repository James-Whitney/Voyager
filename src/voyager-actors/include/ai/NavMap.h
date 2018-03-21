#pragma once
#ifndef _NAVMAP_H
#define _NAVMAP_H

#include <unordered_map>
#include <vector>

#include <glm/glm/gtc/type_ptr.hpp>

#include <voyager-core/include/Entity.h>
#include <voyager-utils/include/Terrain.h>

class Waypoint;

typedef std::shared_ptr<Waypoint> wpt_ptr_t;
typedef std::vector<wpt_ptr_t> wpt_list_t;
typedef std::unordered_map<int, wpt_list_t> nav_map_graph_t;

typedef std::shared_ptr<Terrain> terrain_ptr_t;

typedef std::unordered_map<int, float> score_map_t;

class NavMap :
   public std::enable_shared_from_this<NavMap>,
   public nav_map_graph_t
{
public:
   NavMap(ent_ptr_t player, terrain_ptr_t terrain, btVector3 &terrain_origin, int res, float shift);
   ent_ptr_t getPlayer() { return this->player; }
   std::vector<std::vector<wpt_ptr_t>> getWaypointGrid() { return this->waypoint_grid; }
   void addWaypoint(wpt_ptr_t wpt);
   void makeEdge(wpt_ptr_t start, wpt_ptr_t end);
   wpt_list_t navigate(wpt_ptr_t start, wpt_ptr_t end, int max_depth); // A* search
   wpt_ptr_t getNearestWaypoint(const btVector3 &point);
protected:
   float heuristic(wpt_ptr_t start, wpt_ptr_t goal);
   wpt_list_t reconstructPath(std::unordered_map<int, int> came_from, int current);
private:
   ent_ptr_t player;
   terrain_ptr_t terrain;
   std::unordered_map<int, wpt_ptr_t> id_to_waypoint;
   std::vector<std::vector<wpt_ptr_t>> waypoint_grid;
   void addEdge(wpt_ptr_t start, wpt_ptr_t end);
   score_map_t getInitialScoreMap(wpt_ptr_t start, float start_val);
};

class Waypoint {
public:
   Waypoint(glm::vec3 location); // : location(location) {};
   int getId() { return this->id; }
   glm::vec3 getLocation() { return this->location; }
   float distanceTo(wpt_ptr_t other) const;
private:
   int id;
   glm::vec3 location;
};

// class WaypointCompare {
// public:
//    WaypointCompare(score_map_t *f_score);
//    bool operator() (Waypoint, Waypoint);
// private:
//    score_map_t *f_score;
// }

#endif
