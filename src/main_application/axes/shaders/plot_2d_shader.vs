#version 330

uniform mat4 model_view_proj_mat;
layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 p1;
layout(location = 2) in int idx;
uniform vec3 vertex_color;

out vec3 fragment_color;
out vec4 coord_out;

flat out vec3 p1_out;
out vec3 vert_pos;
flat out int triangle_id;

void main()
{
    gl_Position = model_view_proj_mat * vec4(in_vertex.xyz, 1.0);
    coord_out = vec4(in_vertex.xyz, 1.0);
    vert_pos = in_vertex;
    fragment_color = vertex_color;
    triangle_id = idx;
    p1_out = p1;
}
