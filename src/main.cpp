/**
 * Base code
 * Draws two meshes and one ground plane, one mesh has textures, as well
 * as ground plane.
 * Must be fixed to load in mesh with multiple shapes (dummy.obj)
 */

#include <iostream>
//#include <glad/glad.h>


#include "Player.h"
#include "Entity.h"

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"


#define ENEMY_SPAWN_RATE 0.25
#define PLAYER_SIZE 1.5
#define ENTITY_MAX 10

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//imgui API
#include "Hud.h"


using namespace std;
using namespace glm;


class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;
	Player * player = nullptr;
  Hud * hud = nullptr;

	// Our shader program
	shared_ptr<Program> prog;
	shared_ptr<Program> texProg;

	// Shapes to be used (from obj file)
	std::vector<shared_ptr<Shape>> AllShapes;
	std::vector<Entity> Entities;
	//meshes with just one shape
	shared_ptr<Shape> world;
	shared_ptr<Shape> Nef;
	shared_ptr<Shape> Bunny;

	//ground plane info
	GLuint GrndBuffObj, GrndNorBuffObj, GrndTexBuffObj, GIndxBuffObj;
	int gGiboLen;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

	//geometry for texture render
	GLuint quad_VertexArrayID;
	GLuint quad_vertexbuffer;

	//three different textures
	shared_ptr<Texture> texture0;
 	shared_ptr<Texture> texture1;
 	shared_ptr<Texture> texture2;

	int gMat = 0;

	//For each shape, now that they are not resized, they need to be
	//transformed appropriately to the origin and scaled
	//transforms for Nef
	vec3 gTrans = vec3(0);
	float gScale = 1.0;

	//transforms for the world
	vec3 gDTrans = vec3(0);
	float gDScale = 1.0;

	//transforms for bunny
	vec3 gBTrans = vec3(0);
	float gBScale = 1.0;

	bool mouseDown = false;

	double prevTime = 0;

	int score = 0;


	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	void scrollCallback(GLFWwindow* window, double deltaX, double deltaY)
	{

	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			mouseDown = true;
			glfwGetCursorPos(window, &posX, &posY);
			cout << "Pos X " << posX << " Pos Y " << posY << endl;
		}

		if (action == GLFW_RELEASE)
		{
			mouseDown = false;
		}
	}

	void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	// Code to load in the three textures
	void initTex(const std::string& resourceDirectory)
	{
	 	texture0 = make_shared<Texture>();
		texture0->setFilename(resourceDirectory + "/crate.jpg");
		texture0->init();
		texture0->setUnit(0);
		texture0->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		texture1 = make_shared<Texture>();
		texture1->setFilename(resourceDirectory + "/world.jpg");
		texture1->init();
		texture1->setUnit(1);
		texture1->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		texture2 = make_shared<Texture>();
		texture2->setFilename(resourceDirectory + "/grass.jpg");
		texture2->init();
		texture2->setUnit(2);
		texture2->setWrapModes(GL_REPEAT, GL_REPEAT);
	}

	//code to set up the two shaders - a diffuse shader and texture mapping
	void init(const std::string& resourceDirectory)
	{
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		GLSL::checkVersion();
		glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(
			resourceDirectory + "/simple_vert.glsl",
			resourceDirectory + "/simple_frag.glsl");
		if (! prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("MatAmb");
		prog->addUniform("MatDif");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");

		//initialize the textures we might use
		initTex(resourceDirectory);

		texProg = make_shared<Program>();
		texProg->setVerbose(true);
		texProg->setShaderNames(
			resourceDirectory + "/tex_vert.glsl",
			resourceDirectory + "/tex_frag0.glsl");
		if (! texProg->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
 		texProg->addUniform("P");
		texProg->addUniform("V");
		texProg->addUniform("M");
		texProg->addAttribute("vertPos");
		texProg->addAttribute("vertNor");
		texProg->addAttribute("vertTex");
		texProg->addUniform("Texture0");

		player = new Player(width, height, windowManager);

		cout << "Init\n" << endl;

    hud = new Hud(windowManager->getHandle());
	 }

	void initGeom(const std::string& resourceDirectory)
	{
		// Load geometry
		// Some obj files contain material information.
		// We'll ignore them for this assignment.
		// this is the tiny obj shapes - not to be confused with our shapes
		vector<tinyobj::shape_t> TOshapes;
		vector<tinyobj::material_t> objMaterials;

		string errStr;

		// now read in the sphere for the world
		bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr,
						(resourceDirectory + "/sphere.obj").c_str());

		world =  make_shared<Shape>();
		world->createShape(TOshapes[0]);
		world->measure();
		world->init();

		// compute its transforms based on measuring it
		gDTrans = world->min + 0.5f*(world->max - world->min);
		if (world->max.x >world->max.y && world->max.x > world->max.z)
		{
			gDScale = 2.f / (world->max.x-world->min.x);
		}
		else if (world->max.y > world->max.x && world->max.y > world->max.z)
		{
			gDScale = 2.f / (world->max.y-world->min.y);
		}
		else
		{
			gDScale = 2.f / (world->max.z-world->min.z);
		}

		// now read in the Nefertiti model
		rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr,
						(resourceDirectory + "/Nefertiti-10K.obj").c_str());

		Nef = make_shared<Shape>();
		Nef->createShape(TOshapes[0]);
		Nef->measure();
		Nef->init();

		// compute its transforms based on measuring it
		gTrans = Nef->min + 0.5f * (Nef->max - Nef->min);
		if (Nef->max.x > Nef->max.y && Nef->max.x > Nef->max.z)
		{
			gScale = 2.f / (Nef->max.x - Nef->min.x);
		}
		else if (Nef->max.y > Nef->max.x && Nef->max.y > Nef->max.z)
		{
			gScale = 2.f / (Nef->max.y - Nef->min.y);
		}
		else
		{
			gScale = 2.f / (Nef->max.z - Nef->min.z);
		}

		// now read in the Nefertiti model
		rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr,
			(resourceDirectory + "/bunny.obj").c_str());

		Bunny = make_shared<Shape>();
		Bunny->createShape(TOshapes[0]);
		Bunny->measure();
		Bunny->init();

		// compute its transforms based on measuring it
		gBTrans = Bunny->min + 0.5f * (Bunny->max - Bunny->min);
		if (Bunny->max.x > Bunny->max.y && Bunny->max.x > Bunny->max.z)
		{
			gBScale = 2.f / (Bunny->max.x - Bunny->min.x);
		}
		else if (Bunny->max.y > Bunny->max.x && Bunny->max.y > Bunny->max.z)
		{
			gBScale = 2.f / (Bunny->max.y - Bunny->min.y);
		}
		else
		{
			gBScale = 2.f / (Bunny->max.z - Bunny->min.z);
		}

		// Initialize the geometry to render a ground plane
		initQuad();
	}

	/**** geometry set up for ground plane *****/
	void initQuad()
	{
		float g_groundSize = 20;
		float g_groundY = -1.5;

		// A x-z plane at y = g_groundY of dim[-g_groundSize, g_groundSize]^2
		float GrndPos[] = {
			-g_groundSize, g_groundY, -g_groundSize,
			-g_groundSize, g_groundY,  g_groundSize,
			 g_groundSize, g_groundY,  g_groundSize,
			 g_groundSize, g_groundY, -g_groundSize
		};

		float GrndNorm[] = {
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0
		};

		float GrndTex[] = {
			0, 0, // back
			0, 40,
			40, 40,
			40, 0
		};

		unsigned short idx[] = {0, 1, 2, 0, 2, 3};

		GLuint VertexArrayID;
		//generate the VAO
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		gGiboLen = 6;
		glGenBuffers(1, &GrndBuffObj);
		glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);

		glGenBuffers(1, &GrndNorBuffObj);
		glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GrndNorm), GrndNorm, GL_STATIC_DRAW);

		glGenBuffers(1, &GrndTexBuffObj);
		glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

		glGenBuffers(1, &GIndxBuffObj);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
	}

	void renderGround()
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// draw!
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
		glDrawElements(GL_TRIANGLES, gGiboLen, GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}


	void updateEntities(double deltaTime)
	{
		for (int i = 0; i < Entities.size(); i++)
		{
			//update positions
			Entities[i].updatePosition(deltaTime);
			//check collisions
			if (Entities[i].checkCollision(player->getEyePos(), PLAYER_SIZE) && !(Entities[i].isCaught))
			{
				score++;
				printf("Bunny Caught! Score: %d\n", score);
				Entities[i].isCaught = TRUE;
			}

			Entities[i].checkEntityCollision(Entities, i);
		}
	}

	void addEntities(double time)
	{
		int chunkyTime = (int)(time*ENEMY_SPAWN_RATE);
		if (chunkyTime > Entities.size() && Entities.size() < ENTITY_MAX)
		{
			Entity * newEntity = new Entity();
			Entities.push_back(*newEntity);

			bool isCollidingEntities = newEntity->checkEntityCollision(Entities, Entities.size() - 1);
			bool isCollidingPlayer	 = newEntity->checkCollision(player->getEyePos(), PLAYER_SIZE);

			while (isCollidingEntities || isCollidingPlayer)
			{
				if (isCollidingEntities)
					printf("Collided with another Bunny\n");
				if (isCollidingPlayer)
					printf("Collided with player\n");

				printf("trying to place new bunny\n");
				newEntity->randomPosition();
				isCollidingEntities = newEntity->checkEntityCollision(Entities, Entities.size() - 1);
				isCollidingPlayer = newEntity->checkCollision(player->getEyePos(), PLAYER_SIZE);
			}
		}
	}

	void updateConsole(double deltaTime)
	{
		//clear console
		printf("FPS: %.2f Entity Count: %lu Score: %d\n", (1 / deltaTime), Entities.size(), score);
	}

	void render()
	{
		// Get current frame buffer size.
		int width, height;
		double time = glfwGetTime();
		double deltaTime = time - prevTime;
		prevTime = time;

		player->update(time, Entities);

		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

		glViewport(0, 0, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Hud generation
    hud->start();
    hud->textbox("Hello World", 128, 128, 64, 64);
    hud->render();

		/* Leave this code to just draw the meshes alone */
		float aspect = width/(float)height;

		// Create the matrix stacks
		auto P = make_shared<MatrixStack>();
		auto V = make_shared<MatrixStack>();
		auto M = make_shared<MatrixStack>();
		// Apply perspective projection.
		P->pushMatrix();
		P->perspective(45.0f, aspect, 0.01f, 100.0f);

		V->pushMatrix();
		V->lookAt(player->getEyePos(), player->getEyePos() + player->getCurrLook(), Y_AXIS);

		//Console updates:
		updateConsole(deltaTime);
		if (score == ENTITY_MAX)
		{
			printf("YOU WIN!\n");
			return;
		}
		//Update Entities
		addEntities(time);
		updateEntities(deltaTime);

		//Draw Entities
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
		// globle transforms for 'camera' (you will likely wantt to fix this)
		M->pushMatrix();
		{
			M->loadIdentity();
			for (int i = 0; i < Entities.size(); i++)
			{
				M->pushMatrix();
				{
					M->translate(Entities[i].position);
					M->rotate(Entities[i].getRotation(), Y_AXIS);
					M->scale(gBScale);
					M->translate(-1.0f*gBTrans);
					if (Entities[i].isCaught)
						SetMaterial(2);
					else
						SetMaterial(1);
					glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
					Bunny->draw(prog);
				}
				M->popMatrix();
			}
		}
		M->popMatrix();

		// TODO add code for the transforms for the dummy and loop over
		// all the shapes in the dummy to draw it

		prog->unbind();

		texProg->bind();
		glUniformMatrix4fv(texProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(texProg->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
		M->pushMatrix();
		{
			M->loadIdentity();

			/*draw the ground */
			glUniformMatrix4fv(texProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
			texture2->bind(texProg->getUniform("Texture0"));
			renderGround();
		}
		M->popMatrix();
		texProg->unbind();


		V->popMatrix();
		P->popMatrix();
	}

	// helper function to set materials for shading
	void SetMaterial(int i)
	{
		switch (i)
		{
		case 0: //shiny blue plastic
			glUniform3f(prog->getUniform("MatAmb"), 0.02f, 0.04f, 0.2f);
			glUniform3f(prog->getUniform("MatDif"), 0.0f, 0.16f, 0.9f);
			break;
		case 1: // flat grey
			glUniform3f(prog->getUniform("MatAmb"), 0.13f, 0.13f, 0.14f);
			glUniform3f(prog->getUniform("MatDif"), 0.3f, 0.3f, 0.4f);
			break;
		case 2: //brass
			glUniform3f(prog->getUniform("MatAmb"), 0.3294f, 0.2235f, 0.02745f);
			glUniform3f(prog->getUniform("MatDif"), 0.7804f, 0.5686f, 0.11373f);
			break;
		case 3: //copper
			glUniform3f(prog->getUniform("MatAmb"), 0.1913f, 0.0735f, 0.0225f);
			glUniform3f(prog->getUniform("MatDif"), 0.7038f, 0.27048f, 0.0828f);
			break;
		}
	}

};

int main(int argc, char **argv)
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
			resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init();
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(resourceDir);

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
