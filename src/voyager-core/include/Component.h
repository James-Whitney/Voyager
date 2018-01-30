#pragma once
#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <memory>

class Component {

public:

   virtual void update(double delta_time) = 0;

private:

};

#endif
