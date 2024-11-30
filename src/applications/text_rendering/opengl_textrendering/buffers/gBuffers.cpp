#include "gBuffers.h"


gBuffers::gBuffers()
	:vbo(),vao(),ibo()
{
	// Empty constructor
}

gBuffers::~gBuffers()
{
	// Destructor
}

void gBuffers::CreateBuffers(const void* vb_data,
	uint32_t& vb_size,
	const uint32_t* ib_indices,
	uint32_t& ib_count,
	VertexBufferLayout& vb_layout)
{

	vao.createVertexArray();

	// Vertex buffer (vertices and number of vertices * sizeof(float)
	vbo.createVertexBuffer(vb_data, vb_size);

	// Index buffer (indices and number of indices)
	ibo.createIndexBuffer(ib_indices, ib_count);

	// Vertex Array (vertex buffer and vertex buffer layout) 
	vao.AddBuffer(vbo, vb_layout);
}

void gBuffers::Bind() const
{
	// Bind the buffers
	vao.Bind();
	ibo.Bind();
}


void gBuffers::UnBind() const
{
	// Un Bind the buffers
	vao.UnBind();
	ibo.UnBind();
}
