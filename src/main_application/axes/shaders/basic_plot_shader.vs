#version 330

uniform mat4 model_view_proj_mat;
layout(location = 0) in vec3 in_vertex;
uniform vec3 vertex_color;
uniform float point_size;
uniform float min_z;
uniform float max_z;
uniform int use_color;

out vec3 fragment_color;
out vec4 coord_out;
mat4 unitMat()
{
    mat4 rot_mat;
    for(int r = 0; r < 4; r++)
    {
        for(int c = 0; c < 4; c++)
        {
            rot_mat[r][c] = 0.0;
        }
    }

    rot_mat[0][0] = 1.0; rot_mat[1][1] = 1.0; rot_mat[2][2] = 1.0; rot_mat[3][3] = 1.0;
    return rot_mat;
}

mat4 rotationMatrixX(float ax)
{
    mat4 r = unitMat();
    r[1][1] = cos(ax);  r[1][2] = -sin(ax);
    r[2][1] = sin(ax);  r[2][2] = cos(ax);
    return r;
}

mat4 rotationMatrixY(float ay)
{
    mat4 r = unitMat();
    r[0][0] = cos(ay);  r[0][2] = sin(ay);
    r[2][0] = -sin(ay);  r[2][2] = cos(ay);
    return r;
}

mat4 rotationMatrixZ(float az)
{
    mat4 r = unitMat();
    r[0][0] = cos(az);  r[0][1] = -sin(az);
    r[1][0] = sin(az);  r[1][1] = cos(az);
    return r;
}

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

    return vec3(r / 255.0, g / 255.0, b / 255.0);
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
    
    gl_PointSize = point_size;
}
