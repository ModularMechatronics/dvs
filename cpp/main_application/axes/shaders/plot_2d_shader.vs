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
flat out vec2 v0_out;
flat out vec2 v1_out;
out vec3 vert_pos;
flat out float length_along_fs;
flat out float line_width_in;
flat out float should_remove_pixels;

uniform float axes_width_vs;
uniform float axes_height_vs;

struct Line2D
{
    float a;
    float b;
    float c;
};

vec2 rotate90deg(vec2 vec_in)
{
    return vec2(-vec_in.y, vec_in.x);
}

Line2D lineFromTwoPoints(vec2 p0, vec2 p1)
{
    Line2D line;

    vec2 v = p1 - p0;

    line.a = -v.y;
    line.b = v.x;
    line.c = -(line.a * p0.x + line.b * p0.y);

    return line;
}

vec2 intersectionOfTwoLines(Line2D line0, Line2D line1)
{
    vec2 p;

    p.x = (line0.b * line1.c - line1.b * line0.c) / (line0.a * line1.b - line1.a * line0.b);
    p.y = -(line0.a * line1.c - line1.a * line0.c) / (line0.a * line1.b - line1.a * line0.b);

    return p;
}

Line2D lineFromPointAndNormalVec(vec2 p, vec2 n)
{
   Line2D line;
   line.a = n.x;
   line.b = n.y; 
   line.c = -(line.a * p.x + line.b * p.y);

   return line;
}

float evalLine(Line2D line, vec2 point)
{
   return line.a * point.x + line.b * point.y + line.c;
}

void main()
{
    vec4 p0_p = model_view_proj_mat * vec4(p0, 0.0, 1.0);
    vec4 p1_p = model_view_proj_mat * vec4(p1, 0.0, 1.0);
    vec4 p2_p = model_view_proj_mat * vec4(p2, 0.0, 1.0);

    vec2 vec_along01 = normalize(p1_p.xy - p0_p.xy);
    vec2 vec_along12 = normalize(p2_p.xy - p1_p.xy);

    // TODO: What to do if two adjecent points are at the same pos?
    line_width_in = line_width;

    vec2 q0 = rotate90deg(vec_along01) * line_width;
    vec2 q1 = rotate90deg(vec_along12) * line_width;

    vec2 p0_0 = -q0;
    vec2 p0_1 = -q0 - vec_along01 ;

    vec2 p1_0 = -q1;
    vec2 p1_1 = -q1 + vec_along12;

    vec2 vp0 = p0_1 - p0_0;
    vec2 vp1 = p1_1 - p1_0;

    vp0 /= length(vp0);
    vp1 /= length(vp1);

    Line2D line0 = lineFromTwoPoints(p0_0, p0_1);
    Line2D line1 = lineFromTwoPoints(p1_0, p1_1);

    vec2 intersection_point = intersectionOfTwoLines(line0, line1);
    vec2 res_vec;

    vec2 q0_normed = q0 / length(q0);
    vec2 q1_normed = q1 / length(q1);
    float dp = dot(vp0, vp1) + 1.0;

    // if(abs(dp) < 0.00001)
    {
        res_vec = -q0_normed * line_width;
    }
    /*else
    {
        res_vec = intersection_point;
    }*/

    p1_out = p1_p.xyz;

    v0_out = vec_along01;
    v1_out = -vec_along12;

    should_remove_pixels = 0.0;

    bool should_flip = false;

    vec3 new_res = cross(vec3(vec_along01.xy, 0.0), vec3(vec_along12.xy, 0.0));

    if(new_res.z > 0.0)
    {
        should_flip = true;
    }
    else
    {
        should_flip = false;
    }

    if(idx < 0.1)  // 0
    {
        res_vec = -q1_normed * line_width;
        gl_Position = vec4(p1_p.xy - res_vec, p1_p.z, 1.0);
        // fragment_color = vec3(1.0, 1.0, 0.0);
        // p1_out = p2_p.xyz;
    }
    else if ((idx > 0.9) && (idx < 1.1)) // 1
    {
        gl_Position = vec4(p1_p.xy - res_vec, p1_p.z, 1.0);
        // fragment_color = vec3(1.0, 0.0, 1.0);
    }
    else if ((idx > 1.9) && (idx < 2.1)) // 2
    {
        // fragment_color = vec3(0.0, 0.0, 1.0);
        gl_Position = vec4(p1_p.xy + res_vec, p1_p.z, 1.0);
    }
    else if ((idx > 2.9) && (idx < 3.1)) // 3
    {
        res_vec = -q1_normed * line_width;
        gl_Position = vec4(p1_p.xy - res_vec, p1_p.z, 1.0);
        // fragment_color = vec3(0.0, 1.0, 1.0);
    }
    else if ((idx > 3.9) && (idx < 4.1)) // 4
    {
        gl_Position = vec4(p1_p.xy + res_vec, p1_p.z, 1.0);
        // fragment_color = vec3(0.0, 0.5, 1.0);
    }
    else if ((idx > 4.9) && (idx < 5.1)) // 5
    {
        res_vec = -q1_normed * line_width;
        gl_Position = vec4(p1_p.xy + res_vec, p1_p.z, 1.0);
        // fragment_color = vec3(1.0, 0.5, 1.0);
    }
    else if ((idx > 5.9) && (idx < 6.1)) // 6
    {
        gl_Position = vec4(p1_p.xy, p1_p.z, 1.0);
        // fragment_color = vec3(1.0, 0.2, 0.6);
    }
    else if ((idx > 6.9) && (idx < 7.1)) // 7
    {
        if(should_flip)
        {
            res_vec = -q0_normed * line_width;
        }
        else
        {
            res_vec = q0_normed * line_width;
        }
        gl_Position = vec4(p1_p.xy + res_vec, p1_p.z, 1.0);
        // fragment_color = vec3(1.0, 0.0, 0.9);
    }
    else if ((idx > 7.9) && (idx < 8.1)) // 8
    {
        if(should_flip)
        {
            res_vec = -q1_normed * line_width;
        }
        else
        {
            res_vec = q1_normed * line_width;
        }
        
        gl_Position = vec4(p1_p.xy + res_vec, p1_p.z, 1.0);
        // fragment_color = vec3(1.0, 0.5, 0.1);
    }
    else if ((idx > 8.9) && (idx < 9.1)) // 9
    {
        if(should_flip)
        {
            res_vec = -q0_normed * line_width;
        }
        else
        {
            res_vec = q0_normed * line_width;
        }
        gl_Position = vec4(p1_p.xy + res_vec, p1_p.z, 1.0);
        // fragment_color = vec3(1.0, 0.0, 0.9);
        should_remove_pixels = 1.0;
    }
    else if ((idx > 9.9) && (idx < 10.1)) // 10
    {
        if(should_flip)
        {
            res_vec = -q1_normed * line_width;
        }
        else
        {
            res_vec = q1_normed * line_width;
        }
        
        gl_Position = vec4(p1_p.xy + res_vec, p1_p.z, 1.0);
        // fragment_color = vec3(1.0, 0.5, 0.1);
        should_remove_pixels = 1.0;
    }
    else if ((idx > 10.9) && (idx < 11.1)) // 11
    {
        if(!should_flip)
        {
            intersection_point = -intersection_point;
        }
        float vec_len = length(intersection_point);
        float real_vec_len = vec_len;
        // float real_vec_len = min(vec_len, line_width * 1.4142135623);
        vec2 intersection_point_normalized = intersection_point * real_vec_len / vec_len;
        gl_Position = vec4(p1_p.xy + intersection_point_normalized, p1_p.z, 1.0);
        // fragment_color = vec3(1.0, 0.5, 0.1);
        should_remove_pixels = 1.0;
    }

    vec4 op = inverse_model_view_proj_mat * gl_Position;

    coord_out = vec4(op.x, op.y, op.z, 1.0);
    fragment_color = vertex_color;
    gl_PointSize = point_size;
    vert_pos     = gl_Position.xyz / gl_Position.w;
    // vert_pos     = vec3(p0.xy, 0.0);
    start_pos    = p0_p.xyz;
    length_along_fs = length_along;

}
