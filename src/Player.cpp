#include "Player.h"

Player::Player(int winWidth, int winHeight, WindowManager* windowManager)
{
	width = winWidth;
	height = winHeight;
	window = windowManager;

	if (window == NULL)
	{
		perror("windowManager is NULL in Player constructor");
		exit(1);
	}
}

void Player::update(double time, std::vector<Entity> Entities)
{
	updateMouse();
	updatePosition(time);
}

void Player::updateMouse()
{
	double deltaX, deltaY;
	double currX, currY;

	glfwGetCursorPos(window->getHandle(), &currX, &currY);

	deltaX = xPos - currX;
	deltaY = yPos - currY;
	xPos = currX;
	yPos = currY;

	theta += deltaY / (height * 2);

	float upperBound = (PI / 2) - (PI / 16);
	float lowerBound = (-PI / 2) + (PI / 16);
	if (theta > upperBound)
		theta = upperBound;
	else if (theta < lowerBound)
		theta = lowerBound;

	alpha += deltaX / width;

	currLook = vec3(cos(theta)*sin(alpha), sin(theta), cos(theta)*cos(alpha));
}

void Player::updatePosition(double currentTime)
{
	float deltaTime = currentTime - previousTime;
	vec3 movement = vec3(0, 0, 0);
	previousTime = currentTime;

	vec3 lookDir;
	lookDir = normalize(currLook);

	// Move forward
	if (glfwGetKey(window->getHandle(), GLFW_KEY_W) == GLFW_PRESS) {
		movement += lookDir;
	}
	// Move backward
	if (glfwGetKey(window->getHandle(), GLFW_KEY_S) == GLFW_PRESS) {
		movement -= lookDir;
	}
	// Strafe right
	if (glfwGetKey(window->getHandle(), GLFW_KEY_A) == GLFW_PRESS) {
		movement -= (cross(lookDir, Y_AXIS));
	}
	// Strafe left
	if (glfwGetKey(window->getHandle(), GLFW_KEY_D) == GLFW_PRESS) {
		movement += (cross(lookDir, Y_AXIS));
	}
	// float up, y axis aligned
	if (glfwGetKey(window->getHandle(), GLFW_KEY_SPACE) == GLFW_PRESS) {
		movement += Y_AXIS;
	}
	// float down, y axis aligned
	if (glfwGetKey(window->getHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		movement -= Y_AXIS;
	}

	eyePos += movement * deltaTime * SPEED;

	if (eyePos.y < 0)
		eyePos.y = 0;
}

int Player::checkEntityCollision(std::vector<Entity> Entities)
{
	for (int i = 0; i < Entities.size(); i++)
	{
		if (Entities[i].checkCollision(eyePos, PLAYER_SIZE))
			return TRUE;
	}
	return FALSE;
}