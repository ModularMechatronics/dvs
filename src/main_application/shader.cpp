#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

Plot2DShader::Plot2DShader(const std::string& vertex_shader, const std::string& fragment_shader, const ShaderSource src)
    : ShaderBase(vertex_shader, fragment_shader, src)
{
    setUniformHandles();
}

void Plot2DShader::setUniformHandles()
{
    uniform_handles.half_line_width = glGetUniformLocation(program_id_, "half_line_width");
    uniform_handles.z_offset = glGetUniformLocation(program_id_, "z_offset");
    uniform_handles.use_dash = glGetUniformLocation(program_id_, "use_dash");
}

ShaderBase::ShaderBase(const std::string& vertex_shader, const std::string& fragment_shader, const ShaderSource src)
{
    if (ShaderSource::FILE == src)
    {
        loadShaderFromFiles(vertex_shader, fragment_shader);
    }
    else if (ShaderSource::CODE == src)
    {
        loadShadersFromSourceCode(vertex_shader, fragment_shader);
    }
    else
    {
        throw std::runtime_error("Invalid option for shader source!");
    }

    setBaseUniformHandles();
}

void ShaderBase::setBaseUniformHandles()
{
    base_uniform_handles.has_custom_transform = glGetUniformLocation(program_id_, "has_custom_transform");
    base_uniform_handles.custom_translation_mat = glGetUniformLocation(program_id_, "custom_translation_mat");
    base_uniform_handles.custom_rotation_mat = glGetUniformLocation(program_id_, "custom_rotation_mat");
    base_uniform_handles.custom_scale_mat = glGetUniformLocation(program_id_, "custom_scale_mat");

    base_uniform_handles.use_clip_plane = glGetUniformLocation(program_id_, "use_clip_plane");

    base_uniform_handles.clip_plane0 = glGetUniformLocation(program_id_, "clip_plane0");
    base_uniform_handles.clip_plane1 = glGetUniformLocation(program_id_, "clip_plane1");
    base_uniform_handles.clip_plane2 = glGetUniformLocation(program_id_, "clip_plane2");
    base_uniform_handles.clip_plane3 = glGetUniformLocation(program_id_, "clip_plane3");
    base_uniform_handles.clip_plane4 = glGetUniformLocation(program_id_, "clip_plane4");
    base_uniform_handles.clip_plane5 = glGetUniformLocation(program_id_, "clip_plane5");

    base_uniform_handles.vertex_color = glGetUniformLocation(program_id_, "vertex_color");
    base_uniform_handles.has_color_vec = glGetUniformLocation(program_id_, "has_color_vec");
    base_uniform_handles.alpha = glGetUniformLocation(program_id_, "alpha");
}

void ShaderBase::loadShaderFromFiles(const std::string& vertex_path, const std::string& fragment_path)
{
    std::string vertex_code, fragment_code;
    // Read the Vertex ShaderBase code from the file
    std::ifstream VertexShaderStream(vertex_path, std::ios::in);
    if (VertexShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        vertex_code = sstr.str();
        VertexShaderStream.close();
    }
    else
    {
        throw std::runtime_error("Not able to open " + vertex_path);
    }

    // Read the Fragment ShaderBase code from the file
    std::ifstream FragmentShaderStream(fragment_path, std::ios::in);
    if (FragmentShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        fragment_code = sstr.str();
        FragmentShaderStream.close();
    }
    else
    {
        throw std::runtime_error("Not able to open " + fragment_path);
    }

    loadShadersFromSourceCode(vertex_code, fragment_code);
}

void ShaderBase::loadShadersFromSourceCode(const std::string& vertex_code, const std::string& fragment_code)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile Vertex ShaderBase
    char const* VertexSourcePointer = vertex_code.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex ShaderBase
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);

        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment ShaderBase
    char const* FragmentSourcePointer = fragment_code.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment ShaderBase
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    program_id_ = glCreateProgram();

    glAttachShader(program_id_, VertexShaderID);
    glAttachShader(program_id_, FragmentShaderID);
    glLinkProgram(program_id_);

    // Check the program
    glGetProgramiv(program_id_, GL_LINK_STATUS, &Result);
    glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(program_id_, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(program_id_, VertexShaderID);
    glDetachShader(program_id_, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
}

void ShaderBase::use() const
{
    glUseProgram(program_id_);
}

void ShaderBase::unUse() const
{
    glUseProgram(0);
}

GLuint ShaderBase::programId() const
{
    return program_id_;
}
