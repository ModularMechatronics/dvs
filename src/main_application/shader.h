#ifndef MAIN_APPLICATION_SHADER_H_
#define MAIN_APPLICATION_SHADER_H_

#include <string>

#include "opengl_low_level/opengl_header.h"

class ShaderBase
{
    enum class ShaderSource
    {
        FILE,
        CODE
    };

protected:
    GLuint program_id_;

    void loadShadersFromSourceCode(const std::string& vertex_code, const std::string& fragment_code);
    void loadShaderFromFiles(const std::string& vertex_path, const std::string& fragment_path);

    ShaderBase(const std::string& vertex_shader, const std::string& fragment_shader, const ShaderSource src);

    void setUniformHandles();

public:
    ShaderBase() = default;

    static ShaderBase createFromFiles(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
    static ShaderBase createFromCode(const char* const vertex_code, const char* const fragment_code);

    GLuint programId() const;

    void use() const;
    void unUse() const;

    struct UniformHandles
    {
        // Custom transform
        GLint has_custom_transform;
        GLint custom_translation_mat;
        GLint custom_rotation_mat;
        GLint custom_scale_mat;

        // Clip planes
        GLint use_clip_plane;

        GLint clip_plane0;
        GLint clip_plane1;
        GLint clip_plane2;
        GLint clip_plane3;
        GLint clip_plane4;
        GLint clip_plane5;
    };

    UniformHandles uniform_handles;
};

struct ShaderCollection
{
    ShaderBase text_shader;
    ShaderBase plot_box_shader;
    ShaderBase scatter_shader;
    ShaderBase basic_plot_shader;
    ShaderBase plot_2d_shader;
    ShaderBase plot_3d_shader;
    ShaderBase img_plot_shader;
    ShaderBase draw_mesh_shader;
    ShaderBase legend_shader;
};

#endif  // MAIN_APPLICATION_SHADER_H_
