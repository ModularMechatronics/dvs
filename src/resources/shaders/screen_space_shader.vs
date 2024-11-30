#version 330

layout(location = 0) in vec3 in_vertex;
uniform vec3 vertex_color;

out vec3 fragment_color;

void main()
{
    gl_Position = vec4(in_vertex.x, in_vertex.y, in_vertex.z, 1.0);

    fragment_color = vertex_color;
}
