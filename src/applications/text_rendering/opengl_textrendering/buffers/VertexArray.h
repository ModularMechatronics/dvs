#pragma once
#include <iostream>
#include <OpenGL/gl3.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void createVertexArray();
	void AddBuffer(const VertexBuffer2& vbo, const VertexBufferLayout& layout);
	void Bind() const;
	void UnBind() const;
private:
	uint32_t va_id;
};