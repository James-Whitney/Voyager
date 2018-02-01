#include "include/RenderEngine.h"

using namespace std;

void RenderEngine::init() {
   this->program = make_shared<Program>();

   program->setVerbose(true);
   program->setShaderNames(
      this->resource_dir + "/uber.vert.glsl",
      this->resource_dir + "/uber.frag.glsl"
   );
   if (!program->init()) {
      cerr << "Failed to initialize program" << endl;
      exit(1);
   }

   program->addUniform("MV");
   program->addUniform("P");
   program->addUniform("MatAmb");
   program->addUniform("MatDif");
   program->addAttribute("vertPos");
   program->addAttribute("vertNor");

   for (int i = 0; i < this->components.size(); ++i) {
      this->components.at(i)->init();
   }

   this->hud = make_shared<Hud>(this->window->getHandle(), this->resource_dir);
}

void RenderEngine::execute(double delta_time) {

   int width, height;
   glfwGetFramebufferSize(this->window->getHandle(), &width, &height);
   float aspect = width / (float)height;

   glViewport(0, 0, width, height);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   this->program->bind();

   shared_ptr<MatrixStack> P = make_shared<MatrixStack>();
   shared_ptr<MatrixStack> MV = make_shared<MatrixStack>();

   P->pushMatrix();
   MV->pushMatrix();

   this->camera->setView(aspect, P, MV);

   for (int i = 0; i < this->components.size(); ++i) {
      this->render(static_pointer_cast<Renderable>(this->components.at(i)), MV);
   }

   MV->popMatrix();
   P->popMatrix();

   hud->start();
   hud->generate();
   hud->render();

   this->program->unbind();

   glfwSwapBuffers(this->window->getHandle());


}

void RenderEngine::render(shared_ptr<Renderable> renderable,
      shared_ptr<MatrixStack> MV) {



}
