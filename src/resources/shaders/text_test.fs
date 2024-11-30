#version 330 core

in vec2 text_coords;
out vec4 color;

uniform sampler2D text_sampler;
uniform vec3 text_color;

// uniform float pane_width;
// uniform float pane_height;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text_sampler, text_coords).r);
    color = vec4(text_color, 1.0) * sampled;
}
