#pragma once
#ifndef _APPLICATIONMAKER_H
#define _APPLICATIONMAKER_H

#include <memory>

#include <voyager-core/include/All.h>

std::shared_ptr<Application> make_application(std::shared_ptr<VoyagerConfig> config);

#endif
