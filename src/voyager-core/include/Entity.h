#pragma once
#ifndef _ENTITY_H
#define _ENTITY_H

#include <memory>
#include <vector>

#include <bullet/src/btBulletDynamicsCommon.h>

#include "Component.h"

class Component;

class Entity;

typedef std::shared_ptr<Entity> ent_ptr_t;
typedef std::vector< ent_ptr_t >::iterator child_iter;
typedef std::vector< std::shared_ptr<Component> >::iterator comp_iter;

class Entity :
   public std::enable_shared_from_this<Entity>,
   private std::vector< std::shared_ptr<Component> >
{

public:

   Entity();

   long getId() { return this->id; }

   std::shared_ptr<Entity> getParent() { return this->parent; }
   void setParent(std::shared_ptr<Entity> parent) { this->parent = parent; }

   std::shared_ptr<btTransform> getTransform(bool absolute = false);
   void setTransform(std::shared_ptr<btTransform> transform) {
      this->transform = transform;
   }

   std::shared_ptr<btVector3> getScale() { return this->scale; }
   void setScale(std::shared_ptr<btVector3> scale) { this->scale = scale; }

   child_iter startChild();
   child_iter endChild();
   void addChild(std::shared_ptr<Entity> child);

   std::vector< std::shared_ptr<Entity> > getCollideList() { return this->collideList; }
   void collide(std::shared_ptr<Entity> other) { this->collideList.push_back(other); }
   void resetCollide() { this->collideList.clear(); }

   void setMask(int mask) { this->mask = mask; }

   int getTeamMask() { return this->team_mask; }
   void setTeamMask(int mask) { this->team_mask = mask; }

   virtual void update(double delta_time);

   virtual void add(std::shared_ptr<Component> component);
   virtual std::shared_ptr<Component> componentAt(int i);
   virtual void removeComponentAt(int i);
   virtual int numComponents();
   virtual bool remove(std::shared_ptr<Component> component);

   void setHealth(float health) { this->health = health; }
   float* getHealth() { return &health; }

protected:

   long id;
   std::shared_ptr<btVector3>     scale;
   std::shared_ptr<btTransform>  transform;

   int mask = 0;
   int team_mask = 0;

   std::vector< std::shared_ptr<Entity> > collideList;

   std::shared_ptr<Entity> parent = nullptr;
   std::vector< std::shared_ptr<Entity> > children;

   float health = 0.0;

};

#endif
