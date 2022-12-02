#version 330

uniform mat4 model_view_proj_mat;
layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_color;
uniform vec3 vertex_color;
uniform float point_size;
uniform int has_color_vec;

out vec3 fragment_color;
out vec4 coord_out;

void main()
{
    gl_Position = model_view_proj_mat * vec4(in_vertex.x, in_vertex.y, in_vertex.z, 1.0);
    coord_out = vec4(in_vertex.x, in_vertex.y, in_vertex.z, 1.0);

    if(has_color_vec == int(1))
    {
        fragment_color = in_color;
    }
    else
    {
        fragment_color = vertex_color;
    }
    
    gl_PointSize = point_size;
}
