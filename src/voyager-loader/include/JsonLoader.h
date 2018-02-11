#pragma once
#ifndef _JSONLOADER_H
#define _JSONLOADER_H

#include <string>
#include <memory>

#include <rapidjson/document.h>

/** given a file path, load the rapidjson document */
rapidjson::Document loadDocument(std::string path);

/** given a rapidjson document, store it to a file */
void storeDocument(rapidjson::Document document, std::string path);

/**
 * abstract JsonLoader of an arbitrary object to/from a file
 */
template <typename T>
class JsonLoader {

public:

   /** given a file path, load the object */
   virtual std::shared_ptr<T> load(std::string path) = 0;

   /** store the object to a file */
   virtual void store(std::shared_ptr<T> thing, std::string path) = 0;

};

#endif
