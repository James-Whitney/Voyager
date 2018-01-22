#pragma  once

#ifndef LAB471_ENTITY_H_INCLUDED
#define LAB471_ENTITY_H_INCLUDED

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "WindowManager.h"

#include <cstdlib>
#include <iostream>
#include <ctime>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.1415926
#define BOARD_SIZE 40
#define ENTITY_HEIGHT -0.5
#define ENTITY_SIZE 1.0
#define ENTITY_SPEED 5

#define TRUE 1
#define FALSE 0

#define RAND(X) (((float)rand()/(RAND_MAX))*(X))

using namespace std;
using namespace glm;

class Entity
{
public:
	Entity();
	~Entity() {};

	void randomPosition();
	void updatePosition(float deltaTime);
	int checkCollision(vec3 target, float targetSize);
	int checkEntityCollision(std::vector<Entity> Entities, int self);

	void randomVelocity();
	float getRotation();

	vec3 position = vec3(0, ENTITY_HEIGHT, 0);
	vec3 velocity = vec3(0);
	float speed = 0;
	int isCaught = FALSE;

};



#endif