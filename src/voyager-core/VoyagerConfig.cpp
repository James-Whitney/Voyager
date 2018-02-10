#include "include/VoyagerConfig.h"

#include <iostream>
#include <sstream>

using namespace std;

void VoyagerConfig::dump() {
   cout << endl << "<VoyagerConfig>" << endl;
   cout << "\tresource_dir: " << this->resource_dir << endl;
   cout << "</VoyagerConfig>" << endl << endl;
}
