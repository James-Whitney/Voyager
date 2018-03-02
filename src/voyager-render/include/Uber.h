#pragma once
#ifndef _UBER_H
#define _UBER_H

#include <memory>

#include <glm/glm/gtc/type_ptr.hpp>

#include <voyager-utils/include/Program.h>

enum UberMode {
   UBER_MODE_SIMPLE = 0, // simple shading
   UBER_MODE_COOK_TORRANCE = 1, // cook-torrance
   UBER_MODE_BLINN_PHONG = 2, // blinn-phong (NOT IMPLEMENTED)
   UBER_MODE_NORMAL = 3, // normal shading
   UBER_MODE_TERRAIN_COOK_TORRANCE = 4 // terrain cook-torrance
};

class Uber {

public:

   UberMode mode = UBER_MODE_SIMPLE;

   virtual void setUniforms(std::shared_ptr<Program> prog) = 0;

};

class SimpleUber : public Uber {

public:

   SimpleUber(glm::vec3 amb, glm::vec3 dif);

   UberMode mode = UBER_MODE_SIMPLE;

   virtual void setUniforms(std::shared_ptr<Program> prog);

private:

   glm::vec3 amb; // all 3 components between 0 and 1
   glm::vec3 dif; // all 3 components between 0 and 1

};

/* TODO: implement */
class BlinnPhongUber : public Uber {

public:

   BlinnPhongUber();

   UberMode mode = UBER_MODE_BLINN_PHONG;

   virtual void setUniforms(std::shared_ptr<Program> prog);

private:

};

class CookTorranceUber : public Uber {

public:

   CookTorranceUber(float opacity, glm::vec3 amb, float roughness, float f0,
      float k);

   UberMode mode = UBER_MODE_COOK_TORRANCE;

   virtual void setUniforms(std::shared_ptr<Program> prog);

private:

   float opacity;       // between 0 and 1
   glm::vec3 amb;       // all 3 components between 0 and 1
   float roughness;     // 0 = smooth, 1 = rough
   float f0;            // reflectance and normal incidence 0 to 1
   float k;             // fraction of diffuse reflection 0 to 1

};

class NormalUber : public Uber {

public:

   NormalUber();

   UberMode mode = UBER_MODE_NORMAL;

   virtual void setUniforms(std::shared_ptr<Program> prog);

private:

};

class TerrainCookTorranceUber : public CookTorranceUber {

public:

   TerrainCookTorranceUber(float opacity, glm::vec3 amb, float roughness, float f0, float k);

   UberMode mode = UBER_MODE_TERRAIN_COOK_TORRANCE;

   virtual void setUniforms(std::shared_ptr<Program> prog);

private:

};

#endif
