#include "include/VoyagerConfig.h"

#include <iostream>
#include <sstream>

using namespace std;

string VoyagerConfig::getApplicationTemplate() {
   return this->getResourceDir() + "/templates/" + (this->type == CLIENT ? "client-app.json" : "server-app.json");
}

void VoyagerConfig::dump() {
   cout << endl << "<VoyagerConfig>" << endl;
   cout << "\tresource_dir: " << this->resource_dir << endl;
   cout << "</VoyagerConfig>" << endl << endl;
}
