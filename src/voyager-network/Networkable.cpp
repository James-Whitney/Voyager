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

/*
const btVector3 & 	      getLinearFactor () const
btQuaternion 	            getOrientation () const
const btVector3 & 	      getLinearVelocity () const
const btVector3 & 	      getAngularVelocity () const
*/
sf::Packet Networkable::packTransform(sf::Packet packet) {
   btRigidBody *body = this->getEntity()->getBody();

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

   // linearFactor, linearVelocity, angularVelocity
   btVector3 vecs[3];
   vecs[0] = body->getLinearFactor();  // const return type?
   vecs[1] = body->getLinearVelocity();  // const return type?
   vecs[2] = body->getAngularVelocity();  // const return type?
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
         packet << vecs[i].m_floats[j];
      }
   }

   btQuaternionFloatData orient;
   body->getOrientation().serializeFloat(orient);
   for (int i = 0; i < 4; i++) {
      packet << orient.m_floats[i];
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
   btRigidBody *body = this->getEntity()->getBody();

   btTransformFloatData data;
   for (int i = 0; i < 4; i++) {
      packet >> data.m_origin.m_floats[i];
   }
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
         packet >> data.m_basis.m_el[i].m_floats[j];
      }
   }
   this->getEntity()->getTransform()->deSerializeFloat(data);

   // linearFactor, totalForce, totalTorque, linearVelocity, angularVelocity
   btVector3 vecs[3];
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
         packet >> vecs[i].m_floats[j];
      }
   }
   body->setLinearFactor(vecs[0]);
   body->setLinearVelocity(vecs[1]);
   body->setAngularVelocity(vecs[2]);


   btQuaternionFloatData orient;
   for (int i = 0; i < 4; i++) {
      packet >> orient.m_floats[i];
   }
   body->getOrientation().deSerializeFloat(orient);
}

/*
void 	setGravity (const btVector3 &acceleration)
void 	setDamping (btScalar lin_damping, btScalar ang_damping)
void 	setMassProps (btScalar mass, const btVector3 &inertia)
void 	setLinearFactor (const btVector3 &linearFactor)
void 	setCenterOfMassTransform (const btTransform &xform)
void 	setInvInertiaDiagLocal (const btVector3 &diagInvInertia)
void 	setSleepingThresholds (btScalar linear, btScalar angular)
void 	setLinearVelocity (const btVector3 &lin_vel)
void 	setAngularVelocity (const btVector3 &ang_vel)
void 	setNewBroadphaseProxy (btBroadphaseProxy *broadphaseProxy)
void 	setMotionState (btMotionState *motionState)
void 	setAngularFactor (const btVector3 &angFac)
void 	setAngularFactor (btScalar angFac)
void 	setFlags (int flags)


const btVector3 & 	      getGravity () const
btScalar 	               getLinearDamping () const
btScalar 	               getAngularDamping () const
btScalar 	               getLinearSleepingThreshold () const
btScalar 	               getAngularSleepingThreshold () const
const btCollisionShape * 	getCollisionShape () const
btCollisionShape * 	      getCollisionShape ()
const btVector3 & 	      getLinearFactor () const
btScalar 	               getInvMass () const
const btMatrix3x3 & 	      getInvInertiaTensorWorld () const
const btVector3 & 	      getTotalForce () const
const btVector3 & 	      getTotalTorque () const
const btVector3 & 	      getInvInertiaDiagLocal () const
const btVector3 & 	      getCenterOfMassPosition () const
btQuaternion 	            getOrientation () const
const btTransform & 	      getCenterOfMassTransform () const
const btVector3 & 	      getLinearVelocity () const
const btVector3 & 	      getAngularVelocity () const
btVector3 	               getVelocityInLocalPoint (const btVector3 &rel_pos) const
btMotionState * 	         getMotionState ()
const btMotionState * 	   getMotionState () const
const btVector3 & 	      getAngularFactor () const
btTypedConstraint * 	      getConstraintRef (int index)
int 	                     getNumConstraintRefs () const
int 	                     getFlags () const
btVector3 	               getLocalInertia () const
*/
