#pragma once
#ifndef _COMPONENT_H
#define _COMPONENT_H

class Component {

public:

   Component();

   long getId() { return this->id; }

   virtual void update(double delta_time) = 0;

protected:

   long id;

};

#endif
