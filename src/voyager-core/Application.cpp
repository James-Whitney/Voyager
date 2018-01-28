#include "include/Application.h"

using namespace std;

Application::Application(ApplicationType type, std::string resource_dir) :
   type(type),
   resource_dir(resource_dir)
{}

void Application::run() {

   this->init();

   // while (!glfwWindowShouldClose(this->getWindowManager()->getHandle())) {

      this->update();

      this->render();

   // }

   this->shutdown();

}

void Application::init() {
   string type = this->type == CLIENT ? "client" : "server";
   cout << "--------==[ Initializing " << type << " ]==--------" << endl;
   // GLSL::checkVersion();
   // glClearColor(0.12f, 0.34f, 0.56f, 1.0f);
   // glEnable(GL_DEPTH_TEST);
   // glfwSetInputMode(this->window->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::update() {
   // glfwPollEvents();

   for (int i = 0; i < this->things.size(); ++i) {
      this->things.at(i)->update();
   }
}

void Application::render() {
   // int width, height;
   // glfwGetFramebufferSize(this->window->getHandle(), &width, &height);
   // float aspect = width / (float)height;

   // glViewport(0, 0, width, height);
   // glBindFramebuffer(GL_FRAMEBUFFER, 0);
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   for (int i = 0; i < this->things.size(); ++i) {
      this->things.at(i)->render();
   }

   // glfwSwapBuffers(this->window->getHandle());
}

void Application::shutdown() {
   cout << "--------==[ Shutting Down ]==--------" << endl;
   // this->window->shutdown();
}
