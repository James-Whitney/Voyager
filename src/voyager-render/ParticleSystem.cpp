#include "include/ParticleSystem.h"

void ParticleSystem::init() {
    this->texture->init();
    this->texture->setWrapModes(GL_REPEAT, GL_REPEAT);

    glGenVertexArrays(1, &VertexArrayID);

    g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	g_particule_color_data = new GLubyte[MaxParticles * 4];

    // The VBO containing the 4 vertices of the particles.
    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
    };
    
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

void ParticleSystem::update(double delta_time) {
    createParticles(delta_time);
    updateParticles(delta_time);
}

int ParticleSystem::FindUnusedParticle(){

    for(int i=LastUsedParticle; i<MaxParticles; i++){
        if (ParticlesContainer[i].life <= 0){
            LastUsedParticle = i;
            return i;
        }
    }

    for(int i=0; i<LastUsedParticle; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }

    return 0;
}

void ParticleSystem::SortParticles(){
    std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

void ParticleSystem::createParticles(double delta) {
    if (!create) {
        return;
    }

    // Generate 10 new particule each millisecond,
    // but limit this to 16 ms (60 fps)
    int newparticles = (int)(delta*10000.0);
    if (newparticles > (int)(0.016f*10000.0)) {
        newparticles = (int)(0.016f*10000.0);
    }

    for(int i=0; i<newparticles; i++){

        int particleIndex = FindUnusedParticle();
        ParticlesContainer[particleIndex].life = 0.25f;
        ParticlesContainer[particleIndex].pos = position;

        float spread = 10.f;
        glm::vec3 maindir = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 randomdir = glm::vec3(
            (rand()%2000 - 1000.0f)/1000.0f,
            (rand()%2000 - 1000.0f)/1000.0f,
            (rand()%2000 - 1000.0f)/1000.0f
        );

        randomdir = normalize(randomdir);

        ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;

        ParticlesContainer[particleIndex].r = 255;
        ParticlesContainer[particleIndex].g = 255;
        ParticlesContainer[particleIndex].b = 255;
        ParticlesContainer[particleIndex].a = 255;

        ParticlesContainer[particleIndex].size = (rand()%1000)/2000.0f + 0.1f;
    }
}

void ParticleSystem::updateParticles(double delta) {
    ParticlesCount = 0;

    vec3 CameraPosition = this->camera->getPosition();
    for(int i=0; i<MaxParticles; i++){

        Particle& p = ParticlesContainer[i]; // shortcut

        if(p.life > 0.0f){

            // Decrease life
            p.life -= delta;
            if (p.life > 0.0f){

                // Simulate simple physics : gravity only, no collisions
                p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
                
                p.g -= 10;
                p.b -= 10;

                if (p.g < 0) {
                    p.g = 0;
                }
                if (p.b < 0) {
                    p.b = 0;
                }
                p.pos += p.speed * (float)delta;
                p.cameraDistance = glm::length( p.pos - CameraPosition );

                // Fill the GPU buffer
                g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
                g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
                g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;

                g_particule_position_size_data[4*ParticlesCount+3] = p.size;

                g_particule_color_data[4*ParticlesCount+0] = p.r;
                g_particule_color_data[4*ParticlesCount+1] = p.g;
                g_particule_color_data[4*ParticlesCount+2] = p.b;
                g_particule_color_data[4*ParticlesCount+3] = p.a;

            } else {
                p.cameraDistance = -1.0f;
            }

            ParticlesCount++;
        }
    }
    SortParticles();
}

void ParticleSystem::drawParticles(mat4 View, mat4 Projection) {
    particleProg->bind();

    mat4 ViewProjectionMatrix = Projection * View;

    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);

    glUniform3f(particleProg->getUniform("CameraRight"), View[0][0], View[1][0], View[2][0]);
    glUniform3f(particleProg->getUniform("CameraUp"), View[0][1], View[1][1], View[2][1]);

    glUniformMatrix4fv(particleProg->getUniform("VP"), 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

    this->texture->bind(particleProg->getUniform("Texture"));
    glBindVertexArray(VertexArrayID);

    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 3rd attribute buffer : particles' colors
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1	

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glDisable(GL_BLEND);

    particleProg->unbind();
}