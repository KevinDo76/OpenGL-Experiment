#pragma once
class vertexBuffer
{
public:
	vertexBuffer();
	vertexBuffer(const vertexBuffer& obj);
	vertexBuffer(const void* data, unsigned int dataSize);
	~vertexBuffer();

	void bind() const;
	void unbind() const;

	unsigned int getID() const;
private:
	unsigned id;
	bool init;
};

