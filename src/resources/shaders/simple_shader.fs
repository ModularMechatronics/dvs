#version 330

out vec4 output_color;
in vec3 fragment_color;
uniform int shader_mode;

void main()
{
    if (shader_mode == int(0))  // Normal
    {
        output_color = vec4(fragment_color, 1.0);
    }
    else if (shader_mode == int(1))  // Button
    {
        output_color = vec4(fragment_color, 1.0);
    }
    else if (shader_mode == int(2))  // Slider
    {
        output_color = vec4(fragment_color, 1.0);
    }
}
