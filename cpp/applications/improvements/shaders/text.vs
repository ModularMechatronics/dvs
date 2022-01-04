#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat2 rotation;
uniform vec2 offset;

void main()
{
    vec2 offseted_vec = vertex.xy - offset;
    vec2 rot_vec = rotation * offseted_vec;
    gl_Position = vec4(rot_vec + offset, 0.0, 1.0);
    TexCoords = vertex.zw;
}
