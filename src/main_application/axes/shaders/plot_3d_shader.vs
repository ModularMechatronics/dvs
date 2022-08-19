#version 330

uniform mat4 model_view_proj_mat;
uniform mat4 inverse_model_view_proj_mat;
layout(location = 0) in vec3 p0;
layout(location = 1) in vec3 p1;
layout(location = 2) in vec3 p2;
layout(location = 3) in float idx;
uniform vec3 vertex_color;
uniform float point_size;
uniform float line_width;

out vec3 fragment_color;
out vec4 coord_out;

void main()
{    
    vec4 p0_p = model_view_proj_mat * vec4(p0, 1.0);
    vec4 p1_p = model_view_proj_mat * vec4(p1, 1.0);
    vec4 p2_p = model_view_proj_mat * vec4(p2, 1.0);

    vec2 v0 = p1_p.xy - p0_p.xy;
    vec2 v1 = p2_p.xy - p1_p.xy;

    vec2 v0p = vec2(-v0.y, v0.x);
    float v0_len = sqrt(v0p.x * v0p.x + v0p.y * v0p.y);
    v0p *= line_width / v0_len;

    vec2 v1p = vec2(-v1.y, v1.x);
    float v1_len = sqrt(v1p.x * v1p.x + v1p.y * v1p.y);
    v1p *= line_width / v1_len;

    if(idx < 0.1)
    {
        gl_Position = vec4(p0_p.xy + v0p, p0_p.z, 1.0);
    }
    else if ((idx > 0.9) && (idx < 1.1))
    {
        gl_Position = vec4(p0_p.xy - v0p, p0_p.z, 1.0);
    }
    else if ((idx > 1.9) && (idx < 2.1))
    {
        gl_Position = vec4(p1_p.xy + v1p, p1_p.z, 1.0);
    }
    else if ((idx > 2.9) && (idx < 3.1))
    {
        gl_Position = vec4(p0_p.xy - v0p, p0_p.z, 1.0);
    }
    else if ((idx > 3.9) && (idx < 4.1))
    {
        gl_Position = vec4(p1_p.xy + v1p, p1_p.z, 1.0);
    }
    else if ((idx > 4.9) && (idx < 5.1))
    {
        gl_Position = vec4(p1_p.xy - v1p, p1_p.z, 1.0);
    }

    vec4 op = inverse_model_view_proj_mat * gl_Position;

    coord_out = vec4(op.x, op.y, op.z, 1.0);
    fragment_color = vertex_color;
    gl_PointSize = point_size;
}
