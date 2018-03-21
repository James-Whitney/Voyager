#include "include/Application.h"


#define _APPLICATION_LOG_LIFECYCLE 0 // set to 1 to log lifecycle events

using namespace std;

void log_life(string msg) {
#if _APPLICATION_LOG_LIFECYCLE
   cout << msg << endl;
#endif
}

Application::Application() {
   this->window = make_shared<WindowManager>();
}

void Application::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GL_TRUE);
   }
   if (static_pointer_cast<RenderEngine>(this->render_engine)->getHud()->inputScreen()) {
      static_pointer_cast<RenderEngine>(this->render_engine)->getHud()->guiKeyCallback(window, key, scancode, action, mods);
   } else {
      glfwSetInputMode(this->window->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   }
}

void Application::mouseCallback(GLFWwindow *window, int button, int action, int mods) {
   if (static_pointer_cast<RenderEngine>(this->render_engine)->getHud()->inputScreen()) {
      glfwSetInputMode(this->window->getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   } else {
      glfwSetInputMode(this->window->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   }
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
      log_life("--------------------==[ LOOP ]==--------------------");
      // Game Update
      double delta_time;

      this->timer.reset();
      while (this->timer.tick(&delta_time)) {
         this->update(delta_time);

         //Physics
         this->physics(delta_time);
         //actors
         this->actors(delta_time);
         assert(this->ai_engine != nullptr);
         this->ai_engine->execute(delta_time);

         this->collisionClean();
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
   stringstream ss;
   ss << "--------==[ Initializing " << type << " ]==--------";
   log_life(ss.str());

   if (this->getType() == CLIENT) {

      if (this->window == nullptr) {
         cerr << "Application has no window" << endl;
         exit(1);
      }
      cout << "initialize window" << endl;
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

      cout << "initialize render engine" << endl;
      this->render_engine->init();

      cout << "initializing ai engine" << endl;
      this->ai_engine->init();

      cout << "client initialization done" << endl;
   }
}

void Application::update(double delta_time) {
   glfwPollEvents();

   for (auto &e : this->things) {
      e.second->update(delta_time);
   }

}

void Application::collisionClean() {
   assert(this->physics_engine != nullptr);
   static_pointer_cast<PhysicsEngine>(this->physics_engine)->clearEntityCollides();
}

void Application::actors(double delta_time) {
   assert(this->actor_engine != nullptr);
   this->actor_engine->removeFlagged();
   this->actor_engine->execute(delta_time);
}

void Application::physics(double delta_time) {
   assert(this->physics_engine != nullptr);
   this->physics_engine->removeFlagged();
   this->physics_engine->execute(delta_time);
}

void Application::render() {
   assert(this->getType() == CLIENT);
   assert(this->render_engine != nullptr);
   this->render_engine->removeFlagged();
   this->render_engine->execute();
}

void Application::shutdown() {
   log_life("--------==[ Shutting Down ]==--------");

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
