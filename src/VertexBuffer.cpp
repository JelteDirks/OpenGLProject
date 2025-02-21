#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(const GLvoid* data, GLsizeiptr size)
{
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &buffer_id);
}

void VertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
}

void VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
