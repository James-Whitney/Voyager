#pragma  once

#ifndef LAB471_PLAYER_H_INCLUDED
#define LAB471_PLAYER_H_INCLUDED

#include "WindowManager.h"
#include "MatrixStack.h"
#include "Entity.h"

#include <glm/gtc/type_ptr.hpp>

#define PI 3.1415926

#define PLAYER_SIZE 1.5

#define X_AXIS vec3(1,0,0)
#define Y_AXIS vec3(0,1,0)
#define Z_AXIS vec3(0,0,1)

#define SPEED 4.0f
#define EYE_HEIGHT 0.3f

#define FALSE 0
#define TRUE 1

using namespace std;
using namespace glm;

class Player
{
public:
	Player(int width, int height, WindowManager* windowManager);
	~Player() {};

	void update(double time, std::vector<Entity> Entities);
	int checkEntityCollision(std::vector<Entity> Entities);

	vec3 getEyePos() { return eyePos; }
	vec3 getCurrLook() { return currLook; }

private:
	WindowManager* window = NULL;
	int width;
	int height;

	double xPos = 0;
	double yPos = 0;

	double theta = 0.45;
	double alpha = 0.5;

	double previousTime = 0;

	vec3 eyePos = vec3(0, EYE_HEIGHT, 0);
	vec3 currLook = vec3(0, 0, -1);

	void updateMouse();
	void updatePosition(double currentTime);
};


#endif // LAB471_PLAYER_H_INCLUDED
