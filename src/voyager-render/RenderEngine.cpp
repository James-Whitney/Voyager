#include "include/RenderEngine.h"

#define _RENDERENGINE_LOG_RENDERS 0 // set to 1 to log rendering

using namespace glm;
using namespace std;

void RenderEngine::initShadows() {
   this->depthResolution = 2048;
   glGenFramebuffers(1, &this->depthBufferId);
   glBindFramebuffer(GL_FRAMEBUFFER, depthBufferId);

   //set up texture
   glGenTextures(1, &this->depthTextureId);
   glBindTexture(GL_TEXTURE_2D, this->depthTextureId);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   int width, height;
   glfwGetFramebufferSize(this->window->getHandle(), &width, &height);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, depthResolution,
      depthResolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

   //bind texture to framebuffer
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
    this->depthTextureId, 0);

    // Set up depth
    GLuint rboDepthID;
    glGenRenderbuffers(1, &rboDepthID);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
     depthResolution, depthResolution);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    GL_RENDERBUFFER, rboDepthID);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Error setting up frame buffer - exiting" << endl;
        exit(0);
    }
}

void RenderEngine::initTerrainTexture() {
   this->terrainTexture = std::make_shared<Texture>();
   this->terrainTexture->setFilename(this->terrainTextureFilename);
   this->terrainTexture->init();
   this->terrainTexture->setWrapModes(GL_REPEAT, GL_REPEAT);
   this->terrainTexture->setUnit(1);
}

void RenderEngine::initTerrainNormalMap() {
   this->terrainNormalMap = std::make_shared<Texture>();
   this->terrainNormalMap->setFilename(this->terrainNormalMapFilename);
   this->terrainNormalMap->init();
   this->terrainNormalMap->setWrapModes(GL_REPEAT, GL_REPEAT);
   this->terrainNormalMap->setUnit(2);
}

void RenderEngine::init() {
   this->program = make_shared<Program>();

   glCullFace(GL_BACK);

   program->setVerbose(true);
   program->setShaderNames(
      this->resource_dir + "/uber.vert.glsl",
      this->resource_dir + "/uber.frag.glsl"
   );
   if (!program->init()) {
      cerr << "Failed to initialize program" << endl;
      exit(1);
   }
   program->addUniform("M");
   program->addUniform("V");
   program->addUniform("P");
   program->addUniform("MatAmb");
   program->addUniform("MatDif");
   program->addUniform("lightPos");
   program->addUniform("shadowV");
   program->addUniform("shadowP");
   program->addUniform("depthTexture");
   program->addUniform("shadowMode");
   program->addUniform("lightColor");
   program->addUniform("uberMode");
   program->addUniform("opacity");
   program->addUniform("roughnessValue");
   program->addUniform("F0");
   program->addUniform("K");
   program->addUniform("terrainTexture");
   program->addUniform("terrainNormalMap");
   program->addUniform("fogColor");
   program->addUniform("fogDensity");

   program->addAttribute("vertPos");
   program->addAttribute("vertNor");

   for (int i = 0; i < this->components.size(); ++i) {
      this->components.at(i)->init();
   }
   cout << "components size: " << this->components.size() << endl;
   this->vfc = make_shared<VFCobj>(&this->components);
   this->hud = make_shared<Hud>(this->window->getHandle(), this->resource_dir);
   this->initShadows();
   this->initTerrainTexture();
   this->initTerrainNormalMap();

   // Initialize the skybox
   this->skybox->init();

   // Initialize the skybox shader
   this->skyboxProgram = std::make_shared<Program>();
   this->skyboxProgram->setVerbose(true);
   this->skyboxProgram->setShaderNames(
      this->resource_dir + "/skybox/skybox.vert.glsl",
      this->resource_dir + "/skybox/skybox.frag.glsl"
   );
   if (!this->skyboxProgram->init()) {
      cerr << "Failed to initialize skybox program" << endl;
      exit(1);
   }
   this->skyboxProgram->addUniform("P");
   this->skyboxProgram->addUniform("V");
   this->skyboxProgram->addUniform("fogColor");
   this->skyboxProgram->addUniform("fogHeight");
}

void RenderEngine::execute(double delta_time) {
#if _RENDERENGINE_LOG_RENDERS
   cout << "-<Rendering>------------------" << endl;
   this->camera->dump();
#endif

   int width, height;
   glfwGetFramebufferSize(this->window->getHandle(), &width, &height);
   float aspect = width / (float)height;

   glViewport(0, 0, depthResolution, depthResolution);

   this->program->bind();
   glUniform1ui(this->program->getUniform("uberMode"), 0);

   std::shared_ptr<Terrain> terrain = static_pointer_cast<Terrain>(static_pointer_cast<Renderable>(this->components.at(0))->getMesh().at(0));
   float maxHeight = terrain->getMaxHeight();
   float size = maxHeight * 2;

   btVector3 terrPos = static_pointer_cast<Renderable>(this->components.at(0))
      ->getEntity()->getTransform()->getOrigin();
   float minY = terrPos.getY();

   vec3 camPos = this->camera->getPosition();
   vec3 lightPos = vec3(camPos.x, minY + size, camPos.z);

   mat4 cam = glm::lookAt(lightPos, vec3(camPos.x, minY, camPos.z),
      vec3(1, 0, 0));
   mat4 ortho = glm::ortho(-size, size, -size, size, 1.f, size);

   glUniformMatrix4fv(this->program->getUniform("shadowP"), 1, GL_FALSE,
      value_ptr(ortho));
   glUniformMatrix4fv(this->program->getUniform("shadowV"), 1, GL_FALSE,
      value_ptr(cam));

   // Render the shadows for all the objects to the depth texture
   glBindFramebuffer(GL_FRAMEBUFFER, this->depthBufferId);
   glClear(GL_DEPTH_BUFFER_BIT);

   glUniform1i(this->program->getUniform("shadowMode"), 1);
   for (int i = 0; i < this->components.size(); ++i) {
      this->render(static_pointer_cast<Renderable>(this->components.at(i)));
   }

   // Bind the depth texture to the uniform "depthTexture"
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, this->depthTextureId);
   glUniform1i(this->program->getUniform("depthTexture"), 0);

   // Bind terrain texture
   this->terrainTexture->bind(this->program->getUniform("terrainTexture"));

   // Bind terrain normal map
   this->terrainNormalMap->bind(this->program->getUniform("terrainNormalMap"));

   shared_ptr<MatrixStack> P = make_shared<MatrixStack>();
   shared_ptr<MatrixStack> V = make_shared<MatrixStack>();


   P->pushMatrix();
   V->pushMatrix();

   this->camera->setView(aspect, P, V);

   glUniformMatrix4fv(this->program->getUniform("P"), 1, GL_FALSE,
      value_ptr(P->topMatrix()));
   glUniformMatrix4fv(this->program->getUniform("V"), 1, GL_FALSE,
      value_ptr(V->topMatrix()));

   // TODO: remove hardcoding light position and color
   glUniform3f(this->program->getUniform("lightPos"),
      lightPos.x, lightPos.y, lightPos.z);
   glUniform3f(this->program->getUniform("lightColor"), 1, 1, 1);

   // Render the components to the screen
   glViewport(0, 0, width, height);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   hud->start();

   glUniform1i(this->program->getUniform("shadowMode"), 0);

   // Set fog properties
   Skybox::Fog fog = this->skybox->fog;
   glUniform3f(this->program->getUniform("fogColor"),
      fog.color.x, fog.color.y, fog.color.z);
   glUniform1f(this->program->getUniform("fogDensity"), fog.density);

   // Draw skybox
   this->program->unbind();
   this->skyboxProgram->bind();
      // Bind P and V
      glUniformMatrix4fv(this->skyboxProgram->getUniform("P"), 1, GL_FALSE,
         value_ptr(P->topMatrix()));
      glUniformMatrix4fv(this->skyboxProgram->getUniform("V"), 1, GL_FALSE,
         value_ptr(V->topMatrix()));

      // Bind fog properties
      glUniform3f(this->skyboxProgram->getUniform("fogColor"),
         fog.color.x, fog.color.y, fog.color.z);
      glUniform1f(this->skyboxProgram->getUniform("fogHeight"), fog.height);

      this->skybox->draw();
   this->skyboxProgram->unbind();
   this->program->bind();

   this->vfc->ExtractVFPlanes(P->topMatrix(), V->topMatrix());
   for (auto &idx : this->vfc->ViewFrustCull()) {
      this->render(static_pointer_cast<Renderable>(this->components.at(idx)));
   }
   for (auto &idx : this->vfc->dynamic) {
      this->render(static_pointer_cast<Renderable>(this->components.at(idx)));
   }

   V->popMatrix();
   P->popMatrix();

   if (hud->startScreen) {
      hud->startMenu();
   }  else {
      hud->render();
      hud->shipStats(helm);
   }

   this->program->unbind();
   glfwSwapBuffers(this->window->getHandle());
}

void RenderEngine::render(shared_ptr<Renderable> renderable) {

#if _RENDERENGINE_LOG_RENDERS
   cout << "\trendering component " << renderable->getId() << endl;
#endif

   shared_ptr<MatrixStack> M = make_shared<MatrixStack>();
   M->pushMatrix();
   {
      std::shared_ptr<btTransform> trans = renderable->getEntity()->getTransform();
      M->loadMatrix(bulletToGlm(*trans.get()));
      std::shared_ptr<btVector3> scale = renderable->getEntity()->getScale();
      M->scale(bulletToGlm(*scale.get()) * 2.0f);

      renderable->getUber()->setUniforms(this->program);
      glUniformMatrix4fv(this->program->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));

      for (std::shared_ptr<Shape> shape : renderable->getMesh()) {
         shape->draw(this->program);
      }
   }
   M->popMatrix();
}
