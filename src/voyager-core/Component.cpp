#include "include/Component.h"

long nextId = 0;

Component::Component() :
   id(nextId++)
{}
