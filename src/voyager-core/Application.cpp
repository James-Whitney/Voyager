#include "include/Application.h"

using namespace std;

Application::Application(shared_ptr<VoyagerConfig> config) :
   config(config)
{
   this->window = make_shared<WindowManager>();
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

void Application::resizeCallback(GLFWwindow *window, int in_width, int in_height) {
   glViewport(0, 0, in_width, in_height);
}

void Application::run() {

   this->init();

   while (!this->shouldQuit()) {

      // Game Update
      double delta_time;
      this->timer.reset();
      while (this->timer.tick(&delta_time)) {
         this->update(delta_time);
      }

      // Render
      if (this->getType() == CLIENT) {
         this->render();
      }

   }

   this->shutdown();

}

void Application::init() {
   string type = this->getType() == CLIENT ? "client" : "server";
   cout << "--------==[ Initializing " << type << " ]==--------" << endl;

   if (this->getType() == CLIENT) {

      if (this->window == nullptr) {
         cerr << "Application has no window" << endl;
         exit(1);
      }
      this->window->init(1024, 1024);
      this->window->setEventCallbacks(this);

      GLSL::checkVersion();
      glClearColor(0.12f, 0.34f, 0.56f, 1.0f);
      glEnable(GL_DEPTH_TEST);
      glfwSetInputMode(this->window->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

      if (this->render_engine == nullptr) {
         cerr << "Application has no render_engine" << endl;
         exit(1);
      }
      this->render_engine->init();
   }
}

void Application::update(double delta_time) {
   glfwPollEvents();

   for (auto &e : this->things) {
      e.second->update(delta_time);
   }

}

void Application::render() {
   assert(this->getType() == CLIENT);
   assert(this->render_engine != nullptr);
   this->render_engine->execute();
}

void Application::shutdown() {
   cout << "--------==[ Shutting Down ]==--------" << endl;

   if (this->getType() == CLIENT) {
      this->window->shutdown();
   }
}

bool Application::shouldQuit() {
   if (this->getType() == CLIENT) {
      return glfwWindowShouldClose(this->getWindowManager()->getHandle());
   } else {
      return false;
   }
}
