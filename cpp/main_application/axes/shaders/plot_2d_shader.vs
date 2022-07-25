#version 330

uniform mat4 model_view_proj_mat;
uniform mat4 inverse_model_view_proj_mat;
layout(location = 0) in vec2 p0;
layout(location = 1) in vec2 p1;
layout(location = 2) in vec2 p2;
layout(location = 3) in float idx;
layout(location = 4) in float length_along;
uniform vec3 vertex_color;
uniform float point_size;
uniform float line_width;

out vec3 fragment_color;
out vec4 coord_out;

flat out vec3 start_pos;
flat out vec3 p1_out;
out vec3 vert_pos;
flat out float length_along_fs;
flat out float idx_out;

uniform float axes_width_vs;
uniform float axes_height_vs;

void main()
{
    vec4 p0_p = model_view_proj_mat * vec4(p0, -0.01, 1.0);
    vec4 p1_p = model_view_proj_mat * vec4(p1, -0.01, 1.0);
    vec4 p2_p = model_view_proj_mat * vec4(p2, -0.01, 1.0);

    vec2 v0 = p1_p.xy - p0_p.xy;
    vec2 v1 = p2_p.xy - p1_p.xy;

    v0 = line_width * v0 / length(v0);
    v1 = line_width * v1 / length(v1);

    float sqrt_lw = 1.0; // sqrt(line_width);

    vec2 v0p = vec2(-sqrt_lw * v0.y, sqrt_lw * v0.x);
    vec2 v1p = vec2(-sqrt_lw * v1.y, sqrt_lw * v1.x);

    //////
    float a0 = atan(v0p.y / v0p.x);
    float a1 = atan(v1p.y / v1p.x);
    float a = (a0 + a1) / 2.0f;

    // return Vec2Df(std::cos(a), std::sin(a));
    // vec2 vap = vec2(cos(a), sin(a)) * 0.05;
    vec2 vap = v0p + v1p;
    //////

    // vec2 vap = (v0p + v1p) / 2.0;

    // float ratio = axes_width_vs / axes_height_vs;

    // vec2 vap = vec2(-va.y, va.x);
    // float va_len = sqrt(vap.x * vap.x + vap.y * vap.y);
    // float va_len = length(vap);
    // vap *= line_width / va_len;

    idx_out = idx;

    if(idx < 0.1)  // 0
    {
        gl_Position = vec4(p1_p.xy - vap, p1_p.z, 1.0);
    }
    else if ((idx > 0.9) && (idx < 1.1)) // 1
    {
        gl_Position = vec4(p1_p.xy - vap, p1_p.z, 1.0);
    }
    else if ((idx > 1.9) && (idx < 2.1)) // 2
    {
        gl_Position = vec4(p1_p.xy + vap, p1_p.z, 1.0);
    }
    else if ((idx > 2.9) && (idx < 3.1)) // 3
    {
        gl_Position = vec4(p1_p.xy - vap, p1_p.z, 1.0);
        // gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else if ((idx > 3.9) && (idx < 4.1)) // 4
    {
        gl_Position = vec4(p1_p.xy + vap, p1_p.z, 1.0);
        // gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else if ((idx > 4.9) && (idx < 5.1)) // 5
    {
        gl_Position = vec4(p1_p.xy + vap, p1_p.z, 1.0);
        // gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    }

    vec4 op = inverse_model_view_proj_mat * gl_Position;

    coord_out = vec4(op.x, op.y, op.z, 1.0);
    fragment_color = vertex_color;
    gl_PointSize = point_size;
    vert_pos     = gl_Position.xyz / gl_Position.w;
    // vert_pos     = vec3(p0.xy, 0.0);
    start_pos    = p0_p.xyz;
    p1_out = p1_p.xyz;
    length_along_fs = length_along;
}
