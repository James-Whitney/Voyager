#pragma once
#ifndef _VOYAGERCONFIGLOADER_H
#define _VOYAGERCONFIGLOADER_H

#include <voyager-core/include/VoyagerConfig.h>

#include "JsonLoader.h"

class VoyagerConfigLoader : public JsonLoader<VoyagerConfig> {

public:

   virtual std::shared_ptr<VoyagerConfig> load(std::string path);

   virtual void store(std::shared_ptr<VoyagerConfig> thing, std::string path);

private:

};

#endif
