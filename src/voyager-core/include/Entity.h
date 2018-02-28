#pragma once
#ifndef _ENTITY_H
#define _ENTITY_H

#include <memory>
#include <vector>

#include <bullet/src/btBulletDynamicsCommon.h>

#include "Component.h"

class Entity :
   public std::enable_shared_from_this<Entity>,
   private std::vector< std::shared_ptr<Component> >
{

public:

   Entity();

   long getId() { return this->id; }

   std::shared_ptr<btTransform> getTransform() { return this->transform; }
   void setTransform(std::shared_ptr<btTransform> transform) {
      this->transform = transform;
   }

   std::shared_ptr<btVector3> getScale() { return this->scale; }
   void setScale(std::shared_ptr<btVector3> scale) { this->scale = scale; }

   virtual void update(double delta_time);

   virtual void add(std::shared_ptr<Component> component);
   virtual std::shared_ptr<Component> componentAt(int i);
   virtual int numComponents();
   virtual bool remove(std::shared_ptr<Component> component);

protected:

   long id;
   std::shared_ptr<btVector3>     scale;
   std::shared_ptr<btTransform>  transform;

};

#endif
