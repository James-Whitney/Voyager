#include "Entity.h"

Entity::Entity()
{
	speed = RAND(1) + ENTITY_SPEED;

	randomPosition();
	randomVelocity();
}

void Entity::randomPosition()
{
	float randX = RAND(BOARD_SIZE - 1) - (BOARD_SIZE / 2);
	float randZ = RAND(BOARD_SIZE - 1) - (BOARD_SIZE / 2);
	position = vec3(randX, ENTITY_HEIGHT, randZ);
}

void Entity::updatePosition(float deltaTime)
{
	if (!isCaught)
	{
		vec3 temp = position;
		position += velocity * deltaTime * speed;
		if (abs(position.x) > (BOARD_SIZE / 2))
		{
			position = temp;
			velocity.x *= -1.0;
		}
		if (abs(position.z) > (BOARD_SIZE / 2))
		{
			position = temp;
			velocity.z *= -1.0;
		}
	}
}

int Entity::checkCollision(vec3 target, float targetSize)
{
	float deltaPos = length(target - position);
	if (deltaPos < (targetSize + ENTITY_SIZE))
	{
		return TRUE;
	}
	return FALSE;
}

int Entity::checkEntityCollision(std::vector<Entity> Entities, int self)
{
	for (int i = 0; i < Entities.size(); i++)
	{
		if ( (i != self) && (Entities[self].checkCollision(Entities[i].position, ENTITY_SIZE)) )
		{
			velocity *= -1.0;
			return TRUE;
		}
	}
	return FALSE;
}

void Entity::randomVelocity()
{
	float random = RAND(2*PI);
	velocity = vec3(sin(random), 0, cos(random));
}

float Entity::getRotation()
{
	return atan(velocity.x / velocity.z) + (PI / 2);
}