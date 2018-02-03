#include "include/Component.h"

#include "include/Entity.h"

static long nextId = 0;

Component::Component() :
   id(nextId++)
{}
