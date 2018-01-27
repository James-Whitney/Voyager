#include "Application.h"

using namespace std;

Application::Application(std::string resource_dir) : resource_dir(resource_dir)
{
}

void Application::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GL_TRUE);
   }
}

void Application::mouseCallback(GLFWwindow *window, int button, int action, int mods) {
}

void Application::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
}

void Application::scrollCallback(GLFWwindow *window, double dX, double dY) {

}

void Application::resizeCallback(GLFWwindow *window, int width, int height) {
   glViewport(0, 0, width, height);
}

void Application::run() {

   this->init();

   while (!glfwWindowShouldClose(this->getWindowManager()->getHandle())) {

      this->update();

      this->render();

   }

   this->shutdown();

}

void Application::init() {
   cout << "--------==[ Initializing ]==--------" << endl;
   GLSL::checkVersion();
   glClearColor(0.12f, 0.34f, 0.56f, 1.0f);
   glEnable(GL_DEPTH_TEST);
   glfwSetInputMode(this->window->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   cout << "------------------------------------" << endl;
}

void Application::update() {
   glfwPollEvents();

   for (int i = 0; i < this->things.size(); ++i) {
      this->things.at(i)->update();
   }
}

void Application::render() {
   int width, height;
   glfwGetFramebufferSize(this->window->getHandle(), &width, &height);
   float aspect = width / (float)height;

   glViewport(0, 0, width, height);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   for (int i = 0; i < this->things.size(); ++i) {
      this->things.at(i)->render();
   }

   glfwSwapBuffers(this->window->getHandle());
}

void Application::shutdown() {
   cout << "--------==[ Shutting Down ]==--------" << endl;
   this->window->shutdown();
   cout << "-------------------------------------" << endl;
}
