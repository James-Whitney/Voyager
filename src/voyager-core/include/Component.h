#pragma once
#ifndef _COMPONENT_H
#define _COMPONENT_H

class Component {

public:

   Component();

   virtual void update(double delta_time) = 0;

private:

   long id;

};

#endif
