#include "CollisionBox.h"

bool SphereAABBCollision(const glm::vec3& center, float radius, const glm::vec3& aabbMin, const glm::vec3& aabbMax) {
    float distSquared = 0.0f;
    for (int i = 0; i < 3; i++) {
        if (center[i] < aabbMin[i]) distSquared += (center[i] - aabbMin[i]) * (center[i] - aabbMin[i]);
        else if (center[i] > aabbMax[i]) distSquared += (center[i] - aabbMax[i]) * (center[i] - aabbMax[i]);
    }
    return distSquared <= radius * radius;
}

bool CollisionBox::collidesWithSphere(const glm::vec3& sphereCenter, float radius) const {
    auto aabb = getAABB();
    return SphereAABBCollision(sphereCenter, radius, aabb.first, aabb.second);
}
