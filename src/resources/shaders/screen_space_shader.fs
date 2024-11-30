#version 330

out vec4 output_color;
in vec3 fragment_color;

void main()
{
   output_color = vec4(fragment_color, 1.0);
}
