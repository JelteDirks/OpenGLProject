#ifndef __INDEXBUFFER_H
#define __INDEXBUFFER_H

#include <glad.h>

class IndexBuffer {
private:
	GLuint buffer_id;
public:
	IndexBuffer(const GLuint* indices, const GLsizeiptr size);
	~IndexBuffer();

	void bind();
	void unbind();
};

#endif
