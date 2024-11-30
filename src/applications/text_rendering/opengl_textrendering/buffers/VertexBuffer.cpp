#include "VertexBuffer.h"

VertexBuffer2::VertexBuffer2()
    :vb_id(0)
{
    // Empty constructor at program start
}

VertexBuffer2::~VertexBuffer2()
{
    // Destructor: deletes the vertex buffer object identified by the ID.
    glDeleteBuffers(1, &vb_id);
}


void VertexBuffer2::createVertexBuffer(const void* const data, const uint32_t size)
{
    // Main Constructor
    glGenBuffers(1, &vb_id);
    glBindBuffer(GL_ARRAY_BUFFER, vb_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}


void VertexBuffer2::Bind() const
{
    // Bind this vertex buffer with its id
    glBindBuffer(GL_ARRAY_BUFFER, vb_id);
}

void VertexBuffer2::UnBind() const
{
    // Unbinds the currently bound vertex buffer object.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}