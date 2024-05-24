#pragma once
#include <vector>
#include "GL/glew.h"
#include "vaoElementLayout.h"
#include "vertexBuffer.h"
class vertexArray
{
public:
	vertexArray();
	vertexArray(const vertexArray& id);
	vertexArray(vaoElementLayout layout, vertexBuffer& vb);
	~vertexArray();

	void bind() const;
	void unbind() const;

	unsigned int getID() const; 
private:
	unsigned int id;
	bool init;
};

