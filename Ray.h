#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray {

public:

    Ray(const glm::vec3& origin, const glm::vec3& direction);
    ~Ray();

    const glm::vec3& get_origin() const;
    const glm::vec3& get_direction() const;

    glm::vec3 at(float t) const;

private:

    glm::vec3 origin;
    glm::vec3 direction;
};

#endif
