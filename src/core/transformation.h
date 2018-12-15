#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <memory>
#include <glm/vec3.hpp>

#include "mode.h"
#include "transformationimpl.h"

class transformation
{
private:
    std::unique_ptr<transformationimpl> impl_;
public:
    transformation(std::unique_ptr<transformationimpl> transformationimpl);
    void scale(glm::vec3 scale);
    void rotate(glm::vec3 axis, float angle);
    void translate(glm::vec3 direction);
    void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up);
    void perspective(float fov, float aspect, float near, float far);
};

#endif // TRANSFORMATION_H
