#ifndef PLOT_OBJECTS_UTILS_H_
#define PLOT_OBJECTS_UTILS_H_

#include <iostream>
#include <stdint.h>

#include "enumerations.h"
#include "opengl_low_level/opengl_header.h"

using namespace dvs::internal;

inline size_t getNumDimensionsFromFunction(const Function fcn)
{
    // TODO: This shouldn't be "num dimensions", as it becomes weird when using imShow, which is 2D
    // but it's only one "dimension" (the image), as opposed to surf, which has 3 "dimensions": x, y, z matrices
    // Name it 'num_components'?
    switch(fcn)
    {
        case Function::PLOT2:
            return 2;
        case Function::PLOT3:
            return 3;
        case Function::SCATTER2:
            return 2;
        case Function::SCATTER3:
            return 3;
        case Function::SURF:
            return 3;
        case Function::IM_SHOW:
            return 1;
        case Function::POLYGON_FROM_4_POINTS:
            return 1;
        case Function::PLANE_XY:
            return 1;
        case Function::PLANE_XZ:
            return 1;
        case Function::PLANE_YZ:
            return 1;
        default:
            std::cout << "You haven't defined number of dimensions in utils.h for Function type " << static_cast<int>(fcn) << std::endl;
            exit(-1);
            return -1;
    }
}

inline GLuint LoadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path)
{

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else
    {
        std::cout << "Not able to open " << vertex_file_path << std::endl;
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);

        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

inline GLint dataTypeToGLInt(const DataType& data_type)
{
    GLint gl_type = 0;
    switch(data_type)
    {
        case DataType::FLOAT:
            gl_type = GL_FLOAT;
            break;
        case DataType::DOUBLE:
            gl_type = GL_DOUBLE;
            break;
        case DataType::INT8:
            gl_type = GL_BYTE;
            break;
        case DataType::INT16:
            gl_type = GL_SHORT;
            break;
        case DataType::INT32:
            gl_type = GL_INT;
            break;
        case DataType::INT64:
            gl_type = GL_FLOAT;
            assert(false); // Haven't fround int64 in opengl enums yet...
            break;
        case DataType::UINT8:
            gl_type = GL_UNSIGNED_BYTE;
            break;
        case DataType::UINT16:
            gl_type = GL_UNSIGNED_SHORT;
            break;
        case DataType::UINT32:
            gl_type = GL_UNSIGNED_INT;
            break;
        case DataType::UINT64:
            assert(false); // Haven't fround uint64 in opengl enums yet...
            gl_type = GL_FLOAT;
            break;
        case DataType::UNKNOWN:
            throw std::runtime_error("Got unknown data type!");
            break;
        default:
            throw std::runtime_error("Didn't find valid data type!");
            break;
    }

    return gl_type;
}

#endif
