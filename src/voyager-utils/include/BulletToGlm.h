#pragma once
#ifndef _BULLETTOGLM_H
#define _BULLETTOGLM_H

#include <bullet/src/btBulletDynamicsCommon.h>
#include <glm/glm/gtc/matrix_transform.hpp>

glm::vec3 bulletToGlm(const btVector3& v);
btVector3 glmToBullet(const glm::vec3& v);
/*
glm::quat bulletToGlm(const btQuaternion& q) { return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ()); }

btQuaternion glmToBullet(const glm::quat& q) { return btQuaternion(q.x, q.y, q.z, q.w); }

btMatrix3x3 glmToBullet(const glm::mat3& m) { return btMatrix3x3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2]); }

// btTransform does not contain a full 4x4 matrix, so this transform is lossy.
// Affine transformations are OK but perspective transformations are not.
btTransform glmToBullet(const glm::mat4& m)
{
    glm::mat3 m3(m);
    return btTransform(glmToBullet(m3), glmToBullet(glm::vec3(m[3][0], m[3][1], m[3][2])));
}
*/
glm::mat4 bulletToGlm(const btTransform& t);
#endif
