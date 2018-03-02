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

sf::Packet Networkable::packTransform(sf::Packet packet, std::shared_ptr<btTransform> trans) {
   /*
    * data has the following field types and names
    * - btMatrix3x3FloatData m_basis;
    *   - btVector3FloatData m_el[3];
    *     - float m_floats[4];
    * - btVector3FloatData   m_origin;
    *   - float m_floats[4];
    */
   btTransformFloatData data;
   trans->serializeFloat(data);

   packet << data.m_origin.m_floats[0] << data.m_origin.m_floats[1]
             << data.m_origin.m_floats[2] << data.m_origin.m_floats[3]
          << data.m_basis.m_el[0].m_floats[0] << data.m_basis.m_el[0].m_floats[1]
             << data.m_basis.m_el[0].m_floats[2] << data.m_basis.m_el[0].m_floats[3]
          << data.m_basis.m_el[1].m_floats[0] << data.m_basis.m_el[1].m_floats[1]
             << data.m_basis.m_el[1].m_floats[2] << data.m_basis.m_el[1].m_floats[3]
          << data.m_basis.m_el[2].m_floats[0] << data.m_basis.m_el[2].m_floats[1]
             << data.m_basis.m_el[2].m_floats[2] << data.m_basis.m_el[2].m_floats[3];

   return packet;
}

void Networkable::unpackTransform(sf::Packet packet, std::shared_ptr<btTransform> trans) {
   btTransformFloatData data;
   packet >> data.m_origin.m_floats[0] >> data.m_origin.m_floats[1]
             >> data.m_origin.m_floats[2] >> data.m_origin.m_floats[3]
          >> data.m_basis.m_el[0].m_floats[0] >> data.m_basis.m_el[0].m_floats[1]
             >> data.m_basis.m_el[0].m_floats[2] >> data.m_basis.m_el[0].m_floats[3]
          >> data.m_basis.m_el[1].m_floats[0] >> data.m_basis.m_el[1].m_floats[1]
             >> data.m_basis.m_el[1].m_floats[2] >> data.m_basis.m_el[1].m_floats[3]
          >> data.m_basis.m_el[2].m_floats[0] >> data.m_basis.m_el[2].m_floats[1]
             >> data.m_basis.m_el[2].m_floats[2] >> data.m_basis.m_el[2].m_floats[3];


   trans->deSerializeFloat(data);
}
