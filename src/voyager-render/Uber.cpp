#include "include/Uber.h"

using namespace glm;
using namespace std;

SimpleUber::SimpleUber(vec3 amb, vec3 dif) :
   amb(amb),
   dif(dif)
{
}

void SimpleUber::setUniforms(shared_ptr<Program> prog) {
   glUniform1ui(prog->getUniform("uberMode"), this->mode);

   glUniform3f(prog->getUniform("MatAmb"), this->amb.x, this->amb.y,
      this->amb.z);
   glUniform3f(prog->getUniform("MatDif"), this->dif.x, this->dif.y,
      this->dif.z);
}

BlinnPhongUber::BlinnPhongUber() {
}

void BlinnPhongUber::setUniforms(shared_ptr<Program> prog) {
   glUniform1ui(prog->getUniform("uberMode"), this->mode);


}

CookTorranceUber::CookTorranceUber(float opacity, vec3 amb, float roughness,
      float f0, float k) :
   opacity(opacity),
   amb(amb),
   roughness(roughness),
   f0(f0),
   k(k)
{
}

void CookTorranceUber::setUniforms(shared_ptr<Program> prog) {
   glUniform1ui(prog->getUniform("uberMode"), this->mode);

   glUniform3f(prog->getUniform("MatAmb"), this->amb.x, this->amb.y,
      this->amb.z);
   glUniform1f(prog->getUniform("opacity"), this->opacity);
   glUniform1f(prog->getUniform("roughnessValue"), this->roughness);
   glUniform1f(prog->getUniform("F0"), this->f0);
   glUniform1f(prog->getUniform("K"), this->k);
}

NormalUber::NormalUber() {}

void NormalUber::setUniforms(shared_ptr<Program> prog) {
   glUniform1ui(prog->getUniform("uberMode"), this->mode);
}