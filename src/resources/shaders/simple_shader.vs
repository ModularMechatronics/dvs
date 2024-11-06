#version 330

layout(location = 0) in vec3 in_vertex;
uniform vec3 vertex_color;
uniform float pane_width;
uniform float pane_height;
uniform int shader_mode;

out vec3 fragment_color;
// out vec4 coord_out;

void main()
{
    if (shader_mode == int(0))  // Normal
    {
        // Largest value that a point can have is width - 1, or height - 1
        gl_Position = vec4(2.0 * in_vertex.x / (pane_width - 1.0) - 1.0,
                           1.0 - 2.0 * in_vertex.y / (pane_height - 1.0),
                           in_vertex.z,
                           1.0);

        fragment_color = vertex_color;
    }
    else if (shader_mode == int(1))  // Button
    {
        gl_Position = vec4(2.0 * in_vertex.x / (pane_width - 1.0) - 1.0,
                           1.0 - 2.0 * in_vertex.y / (pane_height - 1.0),
                           in_vertex.z,
                           1.0);

        fragment_color = vertex_color;
    }
    else if (shader_mode == int(2))  // Slider
    {
        gl_Position = vec4(2.0 * in_vertex.x / (pane_width - 1.0) - 1.0,
                           1.0 - 2.0 * in_vertex.y / (pane_height - 1.0),
                           in_vertex.z,
                           1.0);

        fragment_color = vertex_color;
    }
}
