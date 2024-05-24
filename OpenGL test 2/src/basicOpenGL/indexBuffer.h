#pragma once
class indexBuffer
{
public:
	indexBuffer(const void* data, unsigned int dataSize);
	~indexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getID() { return id; }
private:
	unsigned id;
};


