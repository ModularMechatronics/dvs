#ifndef MAIN_APPLICATION_SHADER_H_
#define MAIN_APPLICATION_SHADER_H_

#include <stdexcept>
#include <string>

#include "opengl_low_level/opengl_header.h"

enum class ShaderSource
{
    FILE,
    CODE
};

class Uniform
{
private:
    GLint handle_;

public:
    Uniform() : handle_{-1} {}
    Uniform(const GLuint program_id, const std::string& uniform_name)
    {
        handle_ = glGetUniformLocation(program_id, uniform_name.c_str());
    }

    void setFloat(const float val) const
    {
        glUniform1f(handle_, val);
    }

    void setInt(const int val) const
    {
        glUniform1i(handle_, val);
    }
};

class ShaderBase
{
protected:
    GLuint program_id_;

    void loadShadersFromSourceCode(const std::string& vertex_code, const std::string& fragment_code);
    void loadShaderFromFiles(const std::string& vertex_path, const std::string& fragment_path);

    void setBaseUniformHandles();

public:
    ShaderBase() = default;
    ShaderBase(const std::string& vertex_shader, const std::string& fragment_shader, const ShaderSource src);

    GLuint programId() const;

    void use() const;
    void unUse() const;

    struct BaseUniformHandles
    {
        // Custom transform
        Uniform has_custom_transform;
        GLint custom_translation_mat;
        GLint custom_rotation_mat;
        GLint custom_scale_mat;

        // Clip planes
        Uniform use_clip_plane;

        GLint clip_plane0;
        GLint clip_plane1;
        GLint clip_plane2;
        GLint clip_plane3;
        GLint clip_plane4;
        GLint clip_plane5;

        // Misc
        GLint vertex_color;
        Uniform has_color_vec;

        Uniform alpha;
    };

    BaseUniformHandles base_uniform_handles;
};

class Plot2DShader : public ShaderBase
{
private:
    void setUniformHandles();

public:
    Plot2DShader() = default;
    Plot2DShader(const std::string& vertex_shader, const std::string& fragment_shader, const ShaderSource src);

    struct UniformHandles
    {
        Uniform half_line_width;
        Uniform z_offset;
        Uniform use_dash;
    };

    UniformHandles uniform_handles;
};

class Plot3DShader : public ShaderBase
{
private:
    void setUniformHandles();

public:
    Plot3DShader() = default;
    Plot3DShader(const std::string& vertex_shader, const std::string& fragment_shader, const ShaderSource src);

    struct UniformHandles
    {
        Uniform half_line_width;
        Uniform use_dash;
    };

    UniformHandles uniform_handles;
};

class TextShader : public ShaderBase
{
private:
    void setUniformHandles();

public:
    TextShader() = default;
    TextShader(const std::string& vertex_shader, const std::string& fragment_shader, const ShaderSource src);

    struct UniformHandles
    {
        GLint text_sampler;
        GLint text_color;
    };

    UniformHandles uniform_handles;
};

struct ShaderCollection
{
    TextShader text_shader;
    ShaderBase plot_box_shader;
    ShaderBase scatter_shader;
    ShaderBase basic_plot_shader;
    Plot2DShader plot_2d_shader;
    Plot3DShader plot_3d_shader;
    ShaderBase img_plot_shader;
    ShaderBase draw_mesh_shader;
    ShaderBase legend_shader;
};

#endif  // MAIN_APPLICATION_SHADER_H_
