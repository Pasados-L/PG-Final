#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H

#include <glm/glm.hpp>
#include <vector>

class CollisionBox {
public:
    glm::vec3 center;
    glm::vec3 size;

    CollisionBox(const glm::vec3& center, const glm::vec3& size)
        : center(center), size(size) {
    }

    std::pair<glm::vec3, glm::vec3> getAABB() const {
        glm::vec3 halfSize = size * 0.5f;
        return { center - halfSize, center + halfSize };
    }

    bool collidesWithSphere(const glm::vec3& sphereCenter, float radius) const;
};

#endif
