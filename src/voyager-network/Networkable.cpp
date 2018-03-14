#include "include/Networkable.h"
#include <voyager-core/include/Entity.h>

void Networkable::init() {
   updateThis = false;
}

void Networkable::update(double delta_time) {
   updateThis = true;
}


/*
void btTransform::serializeFloat (struct btTransformFloatData & dataOut)
void btTransform::deSerializeFloat (const struct btTransformFloatData & dataIn);

struct  btTransformFloatData {
   btMatrix3x3FloatData m_basis;
   btVector3FloatData   m_origin;
};
struct  btMatrix3x3FloatData {
   btVector3FloatData m_el[3];
};
struct  btVector3FloatData {
   float m_floats[4];
};
*/

/*
   packet << data.m_origin.m_floats[0] << data.m_origin.m_floats[1]
             << data.m_origin.m_floats[2] << data.m_origin.m_floats[3]
          << data.m_basis.m_el[0].m_floats[0] << data.m_basis.m_el[0].m_floats[1]
             << data.m_basis.m_el[0].m_floats[2] << data.m_basis.m_el[0].m_floats[3]
          << data.m_basis.m_el[1].m_floats[0] << data.m_basis.m_el[1].m_floats[1]
             << data.m_basis.m_el[1].m_floats[2] << data.m_basis.m_el[1].m_floats[3]
          << data.m_basis.m_el[2].m_floats[0] << data.m_basis.m_el[2].m_floats[1]
             << data.m_basis.m_el[2].m_floats[2] << data.m_basis.m_el[2].m_floats[3];
*/
/*
* data has the following field types and names
* - btMatrix3x3FloatData m_basis;
*   - btVector3FloatData m_el[3];
*     - float m_floats[4];
* - btVector3FloatData   m_origin;
*   - float m_floats[4];
*/
sf::Packet Networkable::packTransform(sf::Packet packet) {
   btTransformFloatData data;
   this->getEntity()->getTransform()->serializeFloat(data);

   for (int i = 0; i < 4; i++) {
      packet << data.m_origin.m_floats[i];
   }
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
         packet << data.m_basis.m_el[i].m_floats[j];
      }
   }


   return packet;
}

/*
   packet >> data.m_origin.m_floats[0] >> data.m_origin.m_floats[1]
             >> data.m_origin.m_floats[2] >> data.m_origin.m_floats[3]
          >> data.m_basis.m_el[0].m_floats[0] >> data.m_basis.m_el[0].m_floats[1]
             >> data.m_basis.m_el[0].m_floats[2] >> data.m_basis.m_el[0].m_floats[3]
          >> data.m_basis.m_el[1].m_floats[0] >> data.m_basis.m_el[1].m_floats[1]
             >> data.m_basis.m_el[1].m_floats[2] >> data.m_basis.m_el[1].m_floats[3]
          >> data.m_basis.m_el[2].m_floats[0] >> data.m_basis.m_el[2].m_floats[1]
             >> data.m_basis.m_el[2].m_floats[2] >> data.m_basis.m_el[2].m_floats[3];
*/
void Networkable::unpackTransform(sf::Packet packet) {
   btTransformFloatData data;
   for (int i = 0; i < 4; i++) {
      packet << data.m_origin.m_floats[i];
   }
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
         packet << data.m_basis.m_el[i].m_floats[j];
      }
   }
   this->getEntity()->getTransform()->deSerializeFloat(data);
}
