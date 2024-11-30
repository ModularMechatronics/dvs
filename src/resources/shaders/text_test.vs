#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 text_coords;

uniform float pane_width;
uniform float pane_height;

void main()
{
    gl_Position = vec4(100.0f * vertex.x / pane_width, 100.0f * vertex.y / pane_height, 0.0, 1.0);
    text_coords = vertex.zw;
}
