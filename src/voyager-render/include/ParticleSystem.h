#pragma once
#ifndef _PARTICLESYSTEM_H
#define _PARTICLESYSTEM_H

#include <iostream>
#include <memory>

#include <voyager-core/include/Engine.h>
#include <voyager-core/include/Entity.h>
#include <voyager-core/include/WindowManager.h>

#include <voyager-utils/include/MatrixStack.h>
#include <voyager-utils/include/Program.h>
#include <voyager-utils/include/BulletToGlm.h>
#include <voyager-utils/include/Terrain.h>
#include <voyager-utils/include/Skybox.h>

#include <voyager-hud/include/Hud.h>

//#include "VFCobj.h"
#include <voyager-vfc/include/VFCobj.h>

#include <voyager-actors/include/ShipComponent.h>

#include <bullet/src/btBulletDynamicsCommon.h>

#include "Camera.h"
#include "Renderable.h"

class ParticleSystem : public Component {

public:
	void setPosition(vec3 position) { this->position = position; }
	void setCamera(shared_ptr<Camera> camera) { this->camera = camera; }
	void setProgram(shared_ptr<Program> program) { this->particleProg = program; }
	void setTexture(shared_ptr<Texture> texture) { this->texture = texture; }
	void setCreate(bool create) { this->create = create; }
    virtual void init();

	virtual void update(double delta_time);

    void drawParticles(mat4 View, mat4 Projection);

private:
    int FindUnusedParticle();

    void SortParticles();

    void createParticles(double delta);

    void updateParticles(double delta);

	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r,g,b,a; // Color
		float size, angle, weight;
		float life; // Remaining life of the particle. if < 0 : dead and unused.
		float cameraDistance;

		bool operator<(const Particle& that) const {
			return this->cameraDistance > that.cameraDistance;
		}
	};

	shared_ptr<Camera> camera;
	shared_ptr<Program> particleProg;
	shared_ptr<Texture> texture;
	vec3 position;
	bool create;

	static const int MaxParticles = 100000;
	Particle ParticlesContainer[MaxParticles];
    GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;

    GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
    GLuint VertexArrayID;
    
	int LastUsedParticle;
	int ParticlesCount;
};

#endif