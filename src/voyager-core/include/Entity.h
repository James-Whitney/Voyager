#pragma once
#ifndef _ENTITY_H
#define _ENTITY_H

#include <memory>
#include <vector>

#include <bullet/src/btBulletDynamicsCommon.h>

#include "Component.h"

typedef std::vector< std::shared_ptr<Entity> >::iterator child_iter;
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

   virtual void update(double delta_time);

   virtual void add(std::shared_ptr<Component> component);
   virtual std::shared_ptr<Component> componentAt(int i);
   virtual int numComponents();
   virtual bool remove(std::shared_ptr<Component> component);

protected:

   long id;
   std::shared_ptr<btVector3>     scale;
   std::shared_ptr<btTransform>  transform;

   std::shared_ptr<Entity> parent = nullptr;
   std::vector< std::shared_ptr<Entity> > children;


};

#endif
