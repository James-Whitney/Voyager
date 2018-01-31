#include "include/Component.h"

static long nextId = 0;

Component::Component() :
   id(nextId++)
{}
