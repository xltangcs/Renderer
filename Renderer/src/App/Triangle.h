#pragma once

#include <glm/glm.hpp>

// �����ζ���
struct Triangle {
    std::vector<glm::vec3> vert;    // vert
    std::vector<glm::vec3> normal;    // normal
    std::vector<glm::vec2> uv;    // uv
};