#include "vertexBuffer.h"
#include "GL/glew.h"
#include <iostream>
vertexBuffer::vertexBuffer(const void* data, unsigned int dataSize)
{
    init = true;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
}

vertexBuffer::vertexBuffer()
    :id(0), init(false)
{
    std::cout << "empty init" << "\n";
}

vertexBuffer::vertexBuffer(const vertexBuffer& obj)
{
    this->id = obj.id;
    this->init = true;
    std::cout << "copy " << obj.id << "\n";
}

vertexBuffer::~vertexBuffer()
{
    glDeleteBuffers(1, &id);
}

void vertexBuffer::bind() const
{
    if (init)
        glBindBuffer(GL_ARRAY_BUFFER, id);
}

void vertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int vertexBuffer::getID() const
{
    return id;
}
