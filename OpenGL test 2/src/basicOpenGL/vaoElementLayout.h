#pragma once
#include <GL/glew.h>
#include <vector>
struct vaoElement
{
	unsigned int vCount;
	unsigned int type;
	bool normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		throw;
	}
};

class vaoElementLayout
{
public:
	template <typename I>
	void pushElement(unsigned int count, bool normalized) {
		throw;
	}

	template <>
	void pushElement<float>(unsigned int count, bool normalized) {
		elements.push_back({ count, GL_FLOAT , true });
		stride += vaoElement::getSizeOfType(GL_FLOAT) * count;
	}

	template <>
	void pushElement<unsigned int>(unsigned int count, bool normalized) {
		elements.push_back({ count, GL_UNSIGNED_INT , true });
		stride += vaoElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template <>
	void pushElement<unsigned char>(unsigned int count, bool normalized) {
		elements.push_back({ count, GL_UNSIGNED_BYTE , true });
		stride += vaoElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}
	inline const std::vector<vaoElement>& getElements() { return elements; }
	inline unsigned int getStride() { return stride; }
private:
	unsigned int stride;
	std::vector<vaoElement>elements;
};

