#version 330

uniform mat4 model_view_proj_mat;
layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 vertex_color;
uniform float point_size;

out vec3 fragment_color;
out vec4 coord_out;


void main()
{
    mat4 r = model_view_proj_mat;
    gl_Position = vec4(in_vertex.x, in_vertex.y, in_vertex.z, 1.0);

    gl_Position = r * gl_Position;
    // coord_out = gl_Position;
    coord_out = vec4(in_vertex.x, in_vertex.y, in_vertex.z, 1.0);
    
    fragment_color = vertex_color;
    gl_PointSize = point_size;
}
