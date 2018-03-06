#pragma once
#ifndef _BRAINCOMPONENT_H
#define _BRAINCOMPONENT_H

#include <voyager-core/include/Component.h>

#include "Brain.h"

class BrainComponent : Component {
public:
   BrainComponent();
private:
   std::shared_ptr<Brain> brain;
};

#endif
