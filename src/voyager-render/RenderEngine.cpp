#include "include/RenderEngine.h"
#include "include/ViewFrustumCulling.h"

#define _RENDERENGINE_LOG_RENDERS 0 // set to 1 to log rendering

using namespace glm;
using namespace std;

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
   program->addUniform("lightColor");
   program->addUniform("uberMode");
   program->addUniform("opacity");
   program->addUniform("roughnessValue");
   program->addUniform("F0");
   program->addUniform("K");

   program->addAttribute("vertPos");
   program->addAttribute("vertNor");

   for (int i = 0; i < this->components.size(); ++i) {
      this->components.at(i)->init();
   }

   this->hud = make_shared<Hud>(this->window->getHandle(), this->resource_dir);
}

void RenderEngine::execute(double delta_time) {
#if _RENDERENGINE_LOG_RENDERS
   cout << "-<Rendering>------------------" << endl;
   this->camera->dump();
#endif

   int width, height;
   glfwGetFramebufferSize(this->window->getHandle(), &width, &height);
   float aspect = width / (float)height;

   glViewport(0, 0, width, height);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   this->program->bind();
   glUniform1ui(this->program->getUniform("uberMode"), 0);

   shared_ptr<MatrixStack> P = make_shared<MatrixStack>();
   shared_ptr<MatrixStack> V = make_shared<MatrixStack>();
   shared_ptr<MatrixStack> M = make_shared<MatrixStack>();

   P->pushMatrix();
   M->pushMatrix();
   V->pushMatrix();

   this->camera->setView(aspect, P, V);

   glUniformMatrix4fv(this->program->getUniform("P"), 1, GL_FALSE,
      value_ptr(P->topMatrix()));
   glUniformMatrix4fv(this->program->getUniform("V"), 1, GL_FALSE,
      value_ptr(V->topMatrix()));

   // TODO: remove hardcoding light position and color
   glUniform3f(this->program->getUniform("lightPos"), 1, 1, 1);
   glUniform3f(this->program->getUniform("lightColor"), 1, 1, 1);

   hud->start();

   VFC vfc;
   vfc.ExtractVFPlanes(P->topMatrix(), V->topMatrix());
   char buff[1024];
   sprintf(buff, "L: (%.3f %.3f %.3f %.3f)\nR: (%.3f %.3f %.3f %.3f)\nB: (%.3f %.3f %.3f %.3f)\nT: (%.3f %.3f %.3f %.3f)\nN: (%.3f %.3f %.3f %.3f)\nF: (%.3f %.3f %.3f %.3f)\n",
      vfc.planes[0].x, vfc.planes[0].y, vfc.planes[0].z, vfc.planes[0].w,
      vfc.planes[1].x, vfc.planes[1].y, vfc.planes[1].z, vfc.planes[1].w,
      vfc.planes[2].x, vfc.planes[2].y, vfc.planes[2].z, vfc.planes[2].w,
      vfc.planes[3].x, vfc.planes[3].y, vfc.planes[3].z, vfc.planes[3].w,
      vfc.planes[4].x, vfc.planes[4].y, vfc.planes[4].z, vfc.planes[4].w,
      vfc.planes[5].x, vfc.planes[5].y, vfc.planes[5].z, vfc.planes[5].w);
   hud->dynamicTextbox("VFCplanes", buff, hud->width*0.25, hud->height*0.5, 1, 1, 1, 1);
   for (int i = 0; i < this->components.size(); ++i) {
      if (vfc.ViewFrustCull(static_pointer_cast<Renderable>(this->components.at(i)), hud, i)) {
         this->render(static_pointer_cast<Renderable>(this->components.at(i)));
      }
   }

   V->popMatrix();
   M->popMatrix();
   P->popMatrix();


   if (hud->startScreen) {
      hud->startMenu();
   } else {
      hud->render(); //hud->run();
   }

   this->program->unbind();

   glfwSwapBuffers(this->window->getHandle());


}

void RenderEngine::render(shared_ptr<Renderable> renderable) {

#if _RENDERENGINE_LOG_RENDERS
   cout << "\trendering component " << renderable->getId() << endl;
#endif

   renderable->getUber()->setUniforms(this->program);
   std::shared_ptr<btTransform> trans = renderable->getEntity()->getTransform();
   glUniformMatrix4fv(this->program->getUniform("M"), 1, GL_FALSE, value_ptr(bulletToGlm(*trans.get())));

   renderable->getShape()->draw(this->program);
}
