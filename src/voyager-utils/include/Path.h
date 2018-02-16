#pragma once
#ifndef _PATH_H
#define _PATH_H

#include <string>

std::string parent_path(std::string path) {
   size_t found;
   found = path.find_last_of("/\\");
   return path.substr(0, found + 1);
}

#endif
