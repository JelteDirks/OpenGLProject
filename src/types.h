#ifndef _TYPES_H
#define _TYPES_H

#include <OpenGL/gltypes.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <variant>

typedef struct Vertex
{
    glm::vec3 pos;
} Vertex;

constexpr int N_VERTICES = 4;
constexpr int N_ELEMENTS = N_VERTICES * 3;
constexpr Vertex vertices[N_VERTICES] =
    {
        { { -1.0f, -1.0f, 0.0f } },
        { {  1.0f, -1.0f, 0.0f } },
        { {  1.0f,  1.0f, 0.0f } },
        { { -1.0f,  1.0f, 0.0f } },
    };
constexpr GLuint elements[N_VERTICES * 3] = { 0, 1, 2, 2, 3, 0 };

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
    SMUN = 4,
    NOOP = 5, /* HACK: keep this as last item for the operation count */
};

enum class CSGShape {
    SPHERE = 0,
    BOX = 1,
    CYL = 2,
    PLANE = 3,
    NOSH = 4, /* HACK: keep this as last item for the shape count */
};

using CSGType = std::variant<CSGShape, CSGOperation>;

#endif
