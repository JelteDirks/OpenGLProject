#ifndef _TYPES_H
#define _TYPES_H

#include <OpenGL/gltypes.h>
#include <cstdint>
#include <string_view>
#include <variant>
#include <array>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


// NOTE: the struct and enums should be the same as the shader definition
typedef struct LinearCSGTreeNode {
	int32_t op;
	int32_t shape;
	float _padding[2];
	glm::vec4 metadata1;  /* see notes below */
	glm::vec4 dimensions;
	glm::mat4 transform;
	glm::vec4 scale;
	glm::vec4 color;
} LinearCSGTreeNode;

enum class CSGOperation {
    UNI = 0,
    INT = 1,
    DIFF = 2,
    MOD = 3,
};

enum class CSGShape {
    SPHERE = 0,
    BOX = 1,
    CYL = 2,
    PLANE = 3,
};

using CSGType = std::variant<CSGShape, CSGOperation>;

constexpr std::array<std::pair<std::string_view, CSGOperation>, 4> operations = {{
    {"Union", CSGOperation::UNI},
    {"Intersection", CSGOperation::INT},
    {"Difference", CSGOperation::DIFF},
    {"Modulus", CSGOperation::MOD},
}};

constexpr std::array<std::pair<std::string_view, CSGShape>, 3> shapes = {{
    {"Sphere", CSGShape::SPHERE},
    {"Box", CSGShape::BOX},
    {"Cyllinder", CSGShape::CYL},
}};

#endif
