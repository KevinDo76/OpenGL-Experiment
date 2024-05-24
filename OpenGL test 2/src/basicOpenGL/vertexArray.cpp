#include "vertexArray.h"
#include "vaoElementLayout.h"
#include <vector>
#include <iostream>
vertexArray::vertexArray(vaoElementLayout layout, vertexBuffer& vb)
{
	init = true;
	vb.bind(); // binding vb for attrib
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	
	const std::vector<vaoElement> elements = layout.getElements();
	int offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		glVertexAttribPointer(i, elements[i].vCount, elements[i].type, elements[i].normalized, layout.getStride(), (const void*)offset);
		glEnableVertexAttribArray(i);
		offset += vaoElement::getSizeOfType(elements[i].type) * elements[i].vCount;
	}
}

vertexArray::vertexArray()
	:id(0), init(false)
{
	std::cout << "empty init" << "\n";
}

vertexArray::vertexArray(const vertexArray& obj)
{
	this->id = obj.id;
	this->init = true;
	std::cout << "copy " << obj.id << "\n";
}

vertexArray::~vertexArray()
{
	glDeleteVertexArrays(1, &id);
}

void vertexArray::bind() const
{
	if (init)
		glBindVertexArray(id);
}

void vertexArray::unbind() const
{
	glBindVertexArray(0);
}

unsigned int vertexArray::getID() const
{
	return id;
}
