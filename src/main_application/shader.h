#ifndef MAIN_APPLICATION_SHADER_H_
#define MAIN_APPLICATION_SHADER_H_

#include <string>

#include "opengl_low_level/opengl_header.h"

namespace shaders
{
// clang-format off
constexpr char* kBasicVertexCode = 
"#version 150 \n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
"void main(){\n"
"    gl_Position.xyz = vertexPosition_modelspace;\n"
"    gl_Position.w = 1.0;\n"
"}\n";

constexpr char* kBasicFragmentCode = 
"#version 150 \n"
"in vec3 fragmentColor;\n"
"out vec3 color;\n"
"void main(){\n"
"	color = vec3(1, 0, 0);\n"
"}\n";

// clang-format on
}  // namespace shaders

class Shader
{
    enum class ShaderSource
    {
        FILE,
        CODE
    };

private:
    GLuint program_id_;

    void loadShadersFromSourceCode(const std::string& vertex_code, const std::string& fragment_code);
    void loadShaderFromFiles(const std::string& vertex_path, const std::string& fragment_path);

    Shader(const std::string& vertex_shader, const std::string& fragment_shader, const ShaderSource src);

public:
    Shader() = default;

    static Shader createFromFiles(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
    static Shader createFromCode(const char* const vertex_code, const char* const fragment_code);

    GLuint programId() const;

    void use() const;
    void unUse() const;
};

struct ShaderCollection
{
    Shader text_shader;
    Shader plot_box_shader;
    Shader surf_shader;
    Shader scatter_shader;
    Shader basic_plot_shader;
    Shader plot_2d_shader;
    Shader plot_3d_shader;
    Shader img_plot_shader;
};

#endif // MAIN_APPLICATION_SHADER_H_
