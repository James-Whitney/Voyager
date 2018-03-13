#pragma once
#ifndef _APPLICATIONMAKER_H
#define _APPLICATIONMAKER_H

#include <memory>

#include <voyager-core/include/All.h>

#include <voyager-physics/include/PhysicsEngine.h>
#include <voyager-render/include/RenderEngine.h>

#include <voyager-actors/include/ActorEngine.h>

#include <voyager-network/include/NetworkEngines.h>

std::shared_ptr<Application> make_application(std::shared_ptr<VoyagerConfig> config);

#endif
