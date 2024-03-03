#pragma once

#include <glm/glm.hpp>

// 三角形定义
struct Triangle {
    glm::vec3 vert[3];    // vert
    glm::vec3 normal[3];    // normal
    glm::vec2 uv[3];    // uv
};