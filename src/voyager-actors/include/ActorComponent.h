#pragma once
#ifndef _ACTORCOMPONENT_H
#define _ACTORCOMPONENT_H

#include <voyager-core/include/Component.h>
#include <voyager-core/include/WindowManager.h>

class ActorComponent : public Component {

public:

   void setWindow(std::shared_ptr<WindowManager> window) { this->window = window; }

protected:

   std::shared_ptr<WindowManager> window;

};

#endif
