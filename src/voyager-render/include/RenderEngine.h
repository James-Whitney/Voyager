#pragma once
#ifndef _RENDERENGINE_H
#define _RENDERENGINE_H

#include <iostream>
#include <memory>

#include <voyager-core/include/Engine.h>
#include <voyager-core/include/Entity.h>
#include <voyager-core/include/WindowManager.h>
#include <voyager-utils/include/MatrixStack.h>
#include <voyager-utils/include/Program.h>
#include <voyager-utils/include/BulletToGlm.h>
#include <voyager-hud/include/Hud.h>

#include <bullet/src/btBulletDynamicsCommon.h>

#include "Camera.h"
#include "Renderable.h"

class RenderEngine : public Engine {

public:

   std::string getResourceDir() { return this->resource_dir; }
   void setResourceDir(std::string resource_dir) {
      this->resource_dir = resource_dir;
   }

   std::shared_ptr<Camera> getCamera() { return this->camera; }
   void setCamera(std::shared_ptr<Camera> camera) { this->camera = camera; }

   std::shared_ptr<Program> getProgram() { return this->program; }
   void setProgram(std::shared_ptr<Program> program) {
      this->program = program;
   }

   std::shared_ptr<WindowManager> getWindow() { return this->window; }
   void setWindow(std::shared_ptr<WindowManager> window) {
      this->window = window;
   }

   std::shared_ptr<Hud> getHud() { return this->hud; }

   virtual void init();
   virtual void execute(double delta_time = 0);

   void setTerrainTexture(std::string filename) { this->terrainTextureFilename = filename; }

protected:

   std::string resource_dir;

   std::shared_ptr<Camera> camera;
   std::shared_ptr<Program> program;
   std::shared_ptr<WindowManager> window;
   std::shared_ptr<Hud> hud;
   GLuint depthBufferId;
   GLuint depthTextureId;

   std::string terrainTextureFilename;
   std::shared_ptr<Texture> terrainTexture;

   virtual void render(std::shared_ptr<Renderable> renderable);

   // virtual void setMaterial(std::shared_ptr<Program> prog, glm::vec3 amb,
   //    glm::vec3 dif);

   void initShadows();
   void initTerrainTexture();
};

#endif
