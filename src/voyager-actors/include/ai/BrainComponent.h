#pragma once
#ifndef _BRAINCOMPONENT_H
#define _BRAINCOMPONENT_H

#include <voyager-core/include/Component.h>

#include "Brain.h"

class BrainComponent : public Component {
public:
   BrainComponent(std::shared_ptr<Brain> brain);
   virtual void init() override;
   virtual void update(double delta_time);
private:
   std::shared_ptr<Brain> brain;
};

#endif
