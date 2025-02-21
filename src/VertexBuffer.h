#ifndef __VERTEXBUFFER_H
#define __VERTEXBUFFER_H

#include <cstddef>
#include <glad.h>

class VertexBuffer {
private:
	GLuint buffer_id;
public:
	VertexBuffer(const GLvoid* data, GLsizeiptr size);
	~VertexBuffer();
	void bind();
	void unbind();
};

#endif
