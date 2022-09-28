#version 330

uniform mat4 model_view_proj_mat;
uniform mat4 rotation_mat;
layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
uniform vec3 vertex_color;
uniform float point_size;
uniform float min_z;
uniform float max_z;
uniform int use_color;

out vec3 fragment_color;
out vec3 frag_normal;
out vec4 coord_out;
out vec3 frag_pos;
flat out vec3 light_pos_in;

vec3 calculateColor(float z_val_norm)
{
    float a = (1.0 - z_val_norm)/0.25;	//invert and group
    int X = int(floor(a)); //this is the integer part
    float Y = floor(255.0 * (a - X)); //fractional part from 0 to 255

    float r, g, b;

    if(X == 0)
    {
        r = 255.0;
        g = Y;
        b = 0.0;
    }
    else if(X == 1)
    {
        r = 255.0 - Y;
        g = 255;
        b = 0;
    }
    else if(X == 2)
    {
        r = 0;
        g = 255;
        b = Y;
    }
    else if(X == 3)
    {
        r = 0;
        g = 255 - Y;
        b = 255;
    }
    else if(X == 4)
    {
        r = 0;
        g = 0;
        b = 255;
    }

    return vec3(r / 455.0, g / 455.0, b / 455.0);
}

void main()
{
    // float pi_2 = 1.57079632679489661923132169163975144209858469968;

    gl_Position = model_view_proj_mat * vec4(in_vertex.x, in_vertex.y, in_vertex.z, 1.0);
    coord_out = vec4(in_vertex.x, in_vertex.y, in_vertex.z, 1.0);
    float delta = max_z - min_z;
    
    if(use_color == 1)
    {
        fragment_color = vertex_color;
    }
    else
    {
        fragment_color = calculateColor((in_vertex.z - min_z) / delta);
    }
    mat4 rm = rotation_mat;
    rm[3][3] = 1;
    vec4 frag_normal_tmp = rm * vec4(in_normal, 1.0);
    frag_normal = frag_normal_tmp.xyz;
    frag_pos = in_vertex;
    vec4 light_pos = vec4(6.0, 6.0, 6.0, 1.0);
    light_pos_in = (rm * light_pos).xyz;
    
    gl_PointSize = point_size;
}
