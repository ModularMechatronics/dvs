#pragma once
#include <iostream>
#include <OpenGL/gl3.h>


class VertexBuffer2
{
public:
	VertexBuffer2();
	~VertexBuffer2();
	void createVertexBuffer(const void* const data, const uint32_t size);
	void Bind() const;
	void UnBind() const;
private:
	uint32_t vb_id;
};
