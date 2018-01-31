#pragma once
#ifndef _RENDERABLE_H
#define _RENDERABLE_H

#include <voyager-core/include/Component.h>

class Renderable : public Component {

public:

   virtual void update(double delta_time);

private:

};

#endif
