#version 330

uniform mat4 model_view_proj_mat;
uniform mat4 inverse_model_view_proj_mat;
layout(location = 0) in vec2 p0;
layout(location = 1) in vec2 p1;
layout(location = 2) in vec2 p2;
layout(location = 3) in int idx;
layout(location = 4) in float length_along;
uniform vec3 vertex_color;
uniform float half_line_width;

out vec3 fragment_color;
out vec4 coord_out;

flat out vec3 p0_out;
flat out vec3 p1_out;
flat out float length_along_fs;
flat out float theta;
out vec3 fragment_pos;
flat out int triangle_id;

struct Line2D
{
    float a;
    float b;
    float c;
};

vec2 rotate90Deg(vec2 vec_in)
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
    // Construct a 2d line from a point on the line 'p', and
    // line normal 'n'
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
    bool should_flip = false;

    // vec_along01 points from point p0 to point p1
    vec2 vec_along01 = normalize(p1 - p0);
    // vec_along12 points from point p1 to point p2
    vec2 vec_along12 = normalize(p2 - p1);

    // Rotate the two vectors so that they are perpendicular to
    // their original direction, thereby reaching the edge of the
    // line.
    vec2 vec_on_line_edge01 = rotate90Deg(vec_along01) * half_line_width;
    vec2 vec_on_line_edge12 = rotate90Deg(vec_along12) * half_line_width;

    // Form two points on the edge, two points per segment
    vec2 point0_on_edge01 = vec_on_line_edge01;
    vec2 point1_on_edge01 = vec_on_line_edge01 - vec_along01;

    vec2 point0_on_edge12 = vec_on_line_edge12;
    vec2 point1_on_edge12 = vec_on_line_edge12 + vec_along12;

    // Fit lines from those two points that are on the edges of each segment
    Line2D line0 = lineFromTwoPoints(point0_on_edge01, point1_on_edge01);
    Line2D line1 = lineFromTwoPoints(point0_on_edge12, point1_on_edge12);

    vec2 intersection_point = intersectionOfTwoLines(line0, line1);

    // The sign of the z component of the cross product of
    // the two vectors depends on the mirroring of the view
    float which_side_vec_z = vec_along01.x * vec_along12.y - vec_along01.y * vec_along12.x;

    if(which_side_vec_z > 0.0)
    {
        should_flip = true;
    }
    else
    {
        should_flip = false;
    }

    // 1st triangle
    if(idx == 0)
    {
        gl_Position = model_view_proj_mat * vec4(p1 + vec_on_line_edge12, 0.0, 1.0);
        triangle_id = 0;
    }
    else if (idx == 1)
    {
        gl_Position = model_view_proj_mat * vec4(p1 - vec_on_line_edge01, 0.0, 1.0);
        triangle_id = 0;
    }
    else if (idx == 2)
    {
        gl_Position = model_view_proj_mat * vec4(p1 + vec_on_line_edge01, 0.0, 1.0);
        triangle_id = 0;
    }
    // 2nd triangle
    else if (idx == 3)
    {
        gl_Position = model_view_proj_mat * vec4(p1 + vec_on_line_edge12, 0.0, 1.0);
        triangle_id = 1;
    }
    else if (idx == 4)
    {
        gl_Position = model_view_proj_mat * vec4(p1 - vec_on_line_edge01, 0.0, 1.0);
        triangle_id = 1;
    }
    else if (idx == 5)
    {
        gl_Position = model_view_proj_mat * vec4(p1 - vec_on_line_edge12, 0.0, 1.0);
        triangle_id = 1;
    }
    // 3rd triangle
    else if (idx == 6)
    {
        gl_Position = model_view_proj_mat * vec4(p1, 0.0, 1.0);
        triangle_id = 2;
    }
    else if (idx == 7)
    {
        triangle_id = 2;

        if(should_flip)
        {
            gl_Position = model_view_proj_mat * vec4(p1 - vec_on_line_edge01, 0.0, 1.0);
        }
        else
        {
            gl_Position = model_view_proj_mat * vec4(p1 + vec_on_line_edge01, 0.0, 1.0);
        }
    }
    else if (idx == 8)
    {
        triangle_id = 2;
        if(should_flip)
        {
            gl_Position = model_view_proj_mat * vec4(p1 - vec_on_line_edge12, 0.0, 1.0);
        }
        else
        {
            gl_Position = model_view_proj_mat * vec4(p1 + vec_on_line_edge12, 0.0, 1.0);
        }
    }
    // 4th triangle
    else if (idx == 9)
    {
        triangle_id = 3;
        if(should_flip)
        {
            gl_Position = model_view_proj_mat * vec4(p1 - vec_on_line_edge01, 0.0, 1.0);
        }
        else
        {
            gl_Position = model_view_proj_mat * vec4(p1 + vec_on_line_edge01, 0.0, 1.0);
        }
    }
    else if (idx == 10)
    {
        triangle_id = 3;
        if(should_flip)
        {
            gl_Position = model_view_proj_mat * vec4(p1 - vec_on_line_edge12, 0.0, 1.0);
        }
        else
        {
            gl_Position = model_view_proj_mat * vec4(p1 + vec_on_line_edge12, 0.0, 1.0);
        }
    }
    else if (idx == 11)
    {
        triangle_id = 3;
        if(should_flip)
        {
            intersection_point = -intersection_point;
        }

        gl_Position = model_view_proj_mat * vec4(p1 + intersection_point, 0.0, 1.0);
    }

    vec4 op = inverse_model_view_proj_mat * gl_Position;

    coord_out = vec4(op.x, op.y, op.z, 1.0);
    fragment_color = vertex_color;
    fragment_pos     = (inverse_model_view_proj_mat * gl_Position).xyz;
    theta = atan(vec_on_line_edge01.y, vec_on_line_edge01.x);

    if(triangle_id == 0)
    {
        if(idx == 0)
        {
            p0_out = vec3(p1, 0.0);
            p1_out = vec3(p2, 0.0);
            length_along_fs = length_along;
        }
        else if (idx == 1)
        {
            p0_out = vec3(p0, 0.0);
            p1_out = vec3(p1, 0.0);
            length_along_fs = length_along - length(p1 - p0);
        }
        else if (idx == 2)
        {
            p0_out = vec3(p0, 0.0);
            p1_out = vec3(p1, 0.0);
            length_along_fs = length_along - length(p1 - p0);
        }
    }
    else if(triangle_id == 1)
    {
        if (idx == 3)
        {
            p0_out = vec3(p1, 0.0);
            p1_out = vec3(p2, 0.0);
            length_along_fs = length_along;
        }
        else if (idx == 4)
        {
            p0_out = vec3(p0, 0.0);
            p1_out = vec3(p1, 0.0);
            length_along_fs = length_along - length(p1 - p0);
        }
        else if (idx == 5)
        {
            p0_out = vec3(p1, 0.0);
            p1_out = vec3(p2, 0.0);
            length_along_fs = length_along;
        }
    }
    else if(triangle_id == 2)
    {
        p0_out = vec3(p0, 0.0);
        p1_out = vec3(p1, 0.0);
        length_along_fs = length_along;
    }
    else if(triangle_id == 3)
    {
        p0_out = vec3(p0, 0.0);
        p1_out = vec3(p1, 0.0);
        length_along_fs = length_along;
    }
    
    
}
