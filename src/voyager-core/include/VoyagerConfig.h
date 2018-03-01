#ifndef _VOYAGERCONFIG_H
#define _VOYAGERCONFIG_H

#include <string>

#include "ApplicationType.h"

class VoyagerConfig {

public:

   /** The type of application (client/server) */
   ApplicationType type = CLIENT;

   /** Where the resources are kept */
   std::string resource_dir;

   /** Getters with default values */
   std::string getResourceDir() { return resource_dir.length() ? resource_dir : "resources"; }
   std::string getApplicationTemplate();

   /** print the contents of the config */
   void dump();

};

#endif
