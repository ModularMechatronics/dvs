#ifndef PLOT_OBJECTS_UTILS_H_
#define PLOT_OBJECTS_UTILS_H_

#include <stdint.h>

#include <iostream>

#include "dvs/enumerations.h"
#include "opengl_low_level/opengl_header.h"

using namespace dvs::internal;

inline size_t getNumDimensionsFromFunction(const Function fcn)
{
    // TODO: This shouldn't be "num dimensions", as it becomes weird when using imShow, which is 2D
    // but it's only one "dimension" (the image), as opposed to surf, which has 3 "dimensions": x, y, z matrices
    // Name it 'num_components'?
    switch (fcn)
    {
        case Function::STAIRS:
            return 2;
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
        case Function::DRAW_TRIANGLES_3D:
            return 3;
        case Function::DRAW_TRIANGLE_3D:
            return 3;
        case Function::DRAW_MESH:
            return 3;
        case Function::DRAW_LINE3D:
            return 3;
        case Function::DRAW_ARROW:
            return 1;
        case Function::DRAW_LINE_BETWEEN_POINTS_3D:
            return 3;
        case Function::DRAW_TILES:
            return 1;

        default:
            std::cout << "You haven't defined number of dimensions in utils.h for Function type "
                      << static_cast<int>(fcn) << std::endl;
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
    char const* VertexSourcePointer = VertexShaderCode.c_str();
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
    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
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
    switch (data_type)
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
            assert(false);  // Haven't fround int64 in opengl enums yet...
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
            assert(false);  // Haven't fround uint64 in opengl enums yet...
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

void fillBufferWithData(uint8_t* const points_ptr,
                        const uint8_t* const data_ptr,
                        const size_t num_elements,
                        const size_t num_bytes_per_element,
                        const size_t num_bytes_for_one_vec)
{
    size_t idx_x = 0;
    size_t idx_y = num_bytes_per_element;
    size_t idx_z = 2 * num_bytes_per_element;
    const size_t num_bytes_per_elementtimes_3 = num_bytes_per_element * 3;
    const size_t num_bytes_for_one_vectimes_2 = num_bytes_for_one_vec * 2;

    for (size_t k = 0; k < num_elements; k++)
    {
        const size_t idx_0 = k * num_bytes_per_element;
        const size_t idx_1 = num_bytes_for_one_vec + idx_0;
        const size_t idx_2 = num_bytes_for_one_vectimes_2 + idx_0;
        const uint8_t* const tmp_ptr_0 = &(data_ptr[idx_0]);
        const uint8_t* const tmp_ptr_1 = &(data_ptr[idx_1]);
        const uint8_t* const tmp_ptr_2 = &(data_ptr[idx_2]);

        for (size_t i = 0; i < num_bytes_per_element; i++)
        {
            points_ptr[idx_x + i] = tmp_ptr_0[i];
            points_ptr[idx_y + i] = tmp_ptr_1[i];
            points_ptr[idx_z + i] = tmp_ptr_2[i];
        }
        idx_x += num_bytes_per_elementtimes_3;
        idx_y += num_bytes_per_elementtimes_3;
        idx_z += num_bytes_per_elementtimes_3;
    }
}

inline std::pair<Vec3Dd, Vec3Dd> findMinMaxFromThreeVectors(uint8_t* const data_buffer,
                                                            const size_t num_elements,
                                                            const size_t num_bytes_for_one_vec,
                                                            const DataType data_type)
{
    Vec3Dd min_vec, max_vec;

    if (data_type == DataType::FLOAT)
    {
        Vector<float> x, y, z;

        x.setInternalData(reinterpret_cast<float*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<float*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        z.setInternalData(reinterpret_cast<float*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);

        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
        z.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::DOUBLE)
    {
        Vector<double> x, y, z;

        x.setInternalData(reinterpret_cast<double*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<double*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        z.setInternalData(reinterpret_cast<double*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);

        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
        z.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::INT8)
    {
        Vector<int8_t> x, y, z;

        x.setInternalData(reinterpret_cast<int8_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<int8_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        z.setInternalData(reinterpret_cast<int8_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);

        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
        z.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::INT16)
    {
        Vector<int16_t> x, y, z;

        x.setInternalData(reinterpret_cast<int16_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<int16_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        z.setInternalData(reinterpret_cast<int16_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);

        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
        z.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::INT32)
    {
        Vector<int32_t> x, y, z;

        x.setInternalData(reinterpret_cast<int32_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<int32_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        z.setInternalData(reinterpret_cast<int32_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);

        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
        z.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::INT64)
    {
        Vector<int64_t> x, y, z;

        x.setInternalData(reinterpret_cast<int64_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<int64_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        z.setInternalData(reinterpret_cast<int64_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);

        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
        z.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::UINT8)
    {
        Vector<uint8_t> x, y, z;

        x.setInternalData(reinterpret_cast<uint8_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<uint8_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        z.setInternalData(reinterpret_cast<uint8_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);

        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
        z.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::UINT16)
    {
        Vector<uint16_t> x, y, z;

        x.setInternalData(reinterpret_cast<uint16_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<uint16_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        z.setInternalData(reinterpret_cast<uint16_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);

        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
        z.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::UINT32)
    {
        Vector<uint32_t> x, y, z;

        x.setInternalData(reinterpret_cast<uint32_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<uint32_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        z.setInternalData(reinterpret_cast<uint32_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);

        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
        z.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::UINT64)
    {
        Vector<uint64_t> x, y, z;

        x.setInternalData(reinterpret_cast<uint64_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<uint64_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        z.setInternalData(reinterpret_cast<uint64_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);

        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
        z.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::UNKNOWN)
    {
        throw std::runtime_error("Unknown data type!");
    }
    else
    {
        throw std::runtime_error("Got default in data type switch!");
    }

    return std::pair<Vec3Dd, Vec3Dd>(min_vec, max_vec);
}

inline std::pair<Vec2Dd, Vec2Dd> findMinMaxFromTwoVectors(uint8_t* const data_buffer,
                                                          const size_t num_elements,
                                                          const size_t num_bytes_for_one_vec,
                                                          const DataType data_type)
{
    Vec2Dd min_vec, max_vec;

    if (data_type == DataType::FLOAT)
    {
        Vector<float> x, y;

        x.setInternalData(reinterpret_cast<float*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<float*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::DOUBLE)
    {
        Vector<double> x, y;

        x.setInternalData(reinterpret_cast<double*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<double*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::INT8)
    {
        Vector<int8_t> x, y;

        x.setInternalData(reinterpret_cast<int8_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<int8_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::INT16)
    {
        Vector<int16_t> x, y;

        x.setInternalData(reinterpret_cast<int16_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<int16_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::INT32)
    {
        Vector<int32_t> x, y;

        x.setInternalData(reinterpret_cast<int32_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<int32_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::INT64)
    {
        Vector<int64_t> x, y;

        x.setInternalData(reinterpret_cast<int64_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<int64_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::UINT8)
    {
        Vector<uint8_t> x, y;

        x.setInternalData(reinterpret_cast<uint8_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<uint8_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::UINT16)
    {
        Vector<uint16_t> x, y;

        x.setInternalData(reinterpret_cast<uint16_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<uint16_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::UINT32)
    {
        Vector<uint32_t> x, y;

        x.setInternalData(reinterpret_cast<uint32_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<uint32_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::UINT64)
    {
        Vector<uint64_t> x, y;

        x.setInternalData(reinterpret_cast<uint64_t*>(data_buffer), num_elements);
        y.setInternalData(reinterpret_cast<uint64_t*>(&(data_buffer[num_bytes_for_one_vec])), num_elements);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        x.setInternalData(nullptr, 0);
        y.setInternalData(nullptr, 0);
    }
    else if (data_type == DataType::UNKNOWN)
    {
        throw std::runtime_error("Unknown data type!");
    }
    else
    {
        throw std::runtime_error("Got default in data type switch!");
    }

    return std::pair<Vec2Dd, Vec2Dd>(min_vec, max_vec);
}

inline std::pair<Vec3Dd, Vec3Dd> findMinMaxFromThreeMatrices(uint8_t* const data_buffer,
                                                             const size_t num_rows,
                                                             const size_t num_cols,
                                                             const size_t num_bytes_for_one_vec,
                                                             const DataType data_type)
{
    Vec3Dd min_vec, max_vec;

    if (data_type == DataType::FLOAT)
    {
        Matrix<float> x, y, z;

        x.setInternalData(reinterpret_cast<float*>(data_buffer), num_rows, num_cols);
        y.setInternalData(reinterpret_cast<float*>(&(data_buffer[num_bytes_for_one_vec])), num_rows, num_cols);
        z.setInternalData(reinterpret_cast<float*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_rows, num_cols);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);
        x.setInternalData(nullptr, 0, 0);
        y.setInternalData(nullptr, 0, 0);
        z.setInternalData(nullptr, 0, 0);
    }
    else if (data_type == DataType::DOUBLE)
    {
        Matrix<double> x, y, z;

        x.setInternalData(reinterpret_cast<double*>(data_buffer), num_rows, num_cols);
        y.setInternalData(reinterpret_cast<double*>(&(data_buffer[num_bytes_for_one_vec])), num_rows, num_cols);
        z.setInternalData(reinterpret_cast<double*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_rows, num_cols);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);
        x.setInternalData(nullptr, 0, 0);
        y.setInternalData(nullptr, 0, 0);
        z.setInternalData(nullptr, 0, 0);
    }
    else if (data_type == DataType::INT8)
    {
        Matrix<int8_t> x, y, z;

        x.setInternalData(reinterpret_cast<int8_t*>(data_buffer), num_rows, num_cols);
        y.setInternalData(reinterpret_cast<int8_t*>(&(data_buffer[num_bytes_for_one_vec])), num_rows, num_cols);
        z.setInternalData(reinterpret_cast<int8_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_rows, num_cols);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);
        x.setInternalData(nullptr, 0, 0);
        y.setInternalData(nullptr, 0, 0);
        z.setInternalData(nullptr, 0, 0);
    }
    else if (data_type == DataType::INT16)
    {
        Matrix<int16_t> x, y, z;

        x.setInternalData(reinterpret_cast<int16_t*>(data_buffer), num_rows, num_cols);
        y.setInternalData(reinterpret_cast<int16_t*>(&(data_buffer[num_bytes_for_one_vec])), num_rows, num_cols);
        z.setInternalData(reinterpret_cast<int16_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_rows, num_cols);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);
        x.setInternalData(nullptr, 0, 0);
        y.setInternalData(nullptr, 0, 0);
        z.setInternalData(nullptr, 0, 0);
    }
    else if (data_type == DataType::INT32)
    {
        Matrix<int32_t> x, y, z;

        x.setInternalData(reinterpret_cast<int32_t*>(data_buffer), num_rows, num_cols);
        y.setInternalData(reinterpret_cast<int32_t*>(&(data_buffer[num_bytes_for_one_vec])), num_rows, num_cols);
        z.setInternalData(reinterpret_cast<int32_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_rows, num_cols);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);
        x.setInternalData(nullptr, 0, 0);
        y.setInternalData(nullptr, 0, 0);
        z.setInternalData(nullptr, 0, 0);
    }
    else if (data_type == DataType::INT64)
    {
        Matrix<int64_t> x, y, z;

        x.setInternalData(reinterpret_cast<int64_t*>(data_buffer), num_rows, num_cols);
        y.setInternalData(reinterpret_cast<int64_t*>(&(data_buffer[num_bytes_for_one_vec])), num_rows, num_cols);
        z.setInternalData(reinterpret_cast<int64_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_rows, num_cols);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);
        x.setInternalData(nullptr, 0, 0);
        y.setInternalData(nullptr, 0, 0);
        z.setInternalData(nullptr, 0, 0);
    }
    else if (data_type == DataType::UINT8)
    {
        Matrix<uint8_t> x, y, z;

        x.setInternalData(reinterpret_cast<uint8_t*>(data_buffer), num_rows, num_cols);
        y.setInternalData(reinterpret_cast<uint8_t*>(&(data_buffer[num_bytes_for_one_vec])), num_rows, num_cols);
        z.setInternalData(reinterpret_cast<uint8_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_rows, num_cols);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);
        x.setInternalData(nullptr, 0, 0);
        y.setInternalData(nullptr, 0, 0);
        z.setInternalData(nullptr, 0, 0);
    }
    else if (data_type == DataType::UINT16)
    {
        Matrix<uint16_t> x, y, z;

        x.setInternalData(reinterpret_cast<uint16_t*>(data_buffer), num_rows, num_cols);
        y.setInternalData(reinterpret_cast<uint16_t*>(&(data_buffer[num_bytes_for_one_vec])), num_rows, num_cols);
        z.setInternalData(reinterpret_cast<uint16_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_rows, num_cols);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);
        x.setInternalData(nullptr, 0, 0);
        y.setInternalData(nullptr, 0, 0);
        z.setInternalData(nullptr, 0, 0);
    }
    else if (data_type == DataType::UINT32)
    {
        Matrix<uint32_t> x, y, z;

        x.setInternalData(reinterpret_cast<uint32_t*>(data_buffer), num_rows, num_cols);
        y.setInternalData(reinterpret_cast<uint32_t*>(&(data_buffer[num_bytes_for_one_vec])), num_rows, num_cols);
        z.setInternalData(reinterpret_cast<uint32_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_rows, num_cols);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);
        x.setInternalData(nullptr, 0, 0);
        y.setInternalData(nullptr, 0, 0);
        z.setInternalData(nullptr, 0, 0);
    }
    else if (data_type == DataType::UINT64)
    {
        Matrix<uint64_t> x, y, z;

        x.setInternalData(reinterpret_cast<uint64_t*>(data_buffer), num_rows, num_cols);
        y.setInternalData(reinterpret_cast<uint64_t*>(&(data_buffer[num_bytes_for_one_vec])), num_rows, num_cols);
        z.setInternalData(reinterpret_cast<uint64_t*>(&(data_buffer[2 * num_bytes_for_one_vec])), num_rows, num_cols);
        min_vec.x = dvs::min(x);
        min_vec.y = dvs::min(y);
        min_vec.z = dvs::min(z);

        max_vec.x = dvs::max(x);
        max_vec.y = dvs::max(y);
        max_vec.z = dvs::max(z);
        x.setInternalData(nullptr, 0, 0);
        y.setInternalData(nullptr, 0, 0);
        z.setInternalData(nullptr, 0, 0);
    }
    else if (data_type == DataType::UNKNOWN)
    {
        throw std::runtime_error("Unknown data type!");
    }
    else
    {
        throw std::runtime_error("Got default in data type switch!");
    }

    return std::pair<Vec3Dd, Vec3Dd>(min_vec, max_vec);
}

template <typename T>
uint8_t* convertData2D(const uint8_t* const input_data,
                       const size_t num_elements,
                       const size_t num_bytes_per_element,
                       const size_t num_bytes_for_one_vec)
{
    uint8_t* output_data = new uint8_t[sizeof(float) * 2 * num_elements];

    float f0, f1;
    uint8_t* f0_data = reinterpret_cast<uint8_t*>(&f0);
    uint8_t* f1_data = reinterpret_cast<uint8_t*>(&f1);
    T t0, t1;
    uint8_t* t0_data = reinterpret_cast<uint8_t*>(&t0);
    uint8_t* t1_data = reinterpret_cast<uint8_t*>(&t1);

    size_t idx_x = 0;
    size_t idx_y = sizeof(float);

    for (size_t k = 0; k < num_elements; k++)
    {
        const size_t idx_0 = k * num_bytes_per_element;
        const size_t idx_1 = num_bytes_for_one_vec + k * num_bytes_per_element;
        const uint8_t* const tmp_ptr_0 = &(input_data[idx_0]);
        const uint8_t* const tmp_ptr_1 = &(input_data[idx_1]);

        for (size_t i = 0; i < num_bytes_per_element; i++)
        {
            t0_data[i] = tmp_ptr_0[i];
            t1_data[i] = tmp_ptr_1[i];
        }

        f0 = t0;
        f1 = t1;

        for (size_t i = 0; i < sizeof(float); i++)
        {
            output_data[idx_x + i] = f0_data[i];
            output_data[idx_y + i] = f1_data[i];
        }
        idx_x += sizeof(float) * 2;
        idx_y += sizeof(float) * 2;
    }

    return output_data;
}

inline uint8_t* convertData2DOuter(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec)
{
    uint8_t* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertData2D<float>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertData2D<double>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertData2D<int8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertData2D<int16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertData2D<int32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertData2D<int64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertData2D<uint8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertData2D<uint16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertData2D<uint32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertData2D<uint64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

template <typename T>
uint8_t* convertData3D(const uint8_t* const input_data,
                       const size_t num_elements,
                       const size_t num_bytes_per_element,
                       const size_t num_bytes_for_one_vec)
{
    uint8_t* output_data = new uint8_t[sizeof(float) * 3 * num_elements];

    float f0, f1, f2;
    uint8_t* f0_data = reinterpret_cast<uint8_t*>(&f0);
    uint8_t* f1_data = reinterpret_cast<uint8_t*>(&f1);
    uint8_t* f2_data = reinterpret_cast<uint8_t*>(&f2);
    T t0, t1, t2;
    uint8_t* t0_data = reinterpret_cast<uint8_t*>(&t0);
    uint8_t* t1_data = reinterpret_cast<uint8_t*>(&t1);
    uint8_t* t2_data = reinterpret_cast<uint8_t*>(&t2);

    size_t idx_x = 0;
    size_t idx_y = sizeof(float);
    size_t idx_z = 2 * sizeof(float);

    for (size_t k = 0; k < num_elements; k++)
    {
        const size_t idx_0 = k * num_bytes_per_element;
        const size_t idx_1 = num_bytes_for_one_vec + k * num_bytes_per_element;
        const size_t idx_2 = num_bytes_for_one_vec * 2 + k * num_bytes_per_element;
        const uint8_t* const tmp_ptr_0 = &(input_data[idx_0]);
        const uint8_t* const tmp_ptr_1 = &(input_data[idx_1]);
        const uint8_t* const tmp_ptr_2 = &(input_data[idx_2]);

        for (size_t i = 0; i < num_bytes_per_element; i++)
        {
            t0_data[i] = tmp_ptr_0[i];
            t1_data[i] = tmp_ptr_1[i];
            t2_data[i] = tmp_ptr_2[i];
        }

        f0 = t0;
        f1 = t1;
        f2 = t2;

        for (size_t i = 0; i < sizeof(float); i++)
        {
            output_data[idx_x + i] = f0_data[i];
            output_data[idx_y + i] = f1_data[i];
            output_data[idx_z + i] = f2_data[i];
        }
        idx_x += sizeof(float) * 3;
        idx_y += sizeof(float) * 3;
        idx_z += sizeof(float) * 3;
    }

    return output_data;
}

inline uint8_t* convertData3DOuter(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec)
{
    uint8_t* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertData3D<float>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertData3D<double>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertData3D<int8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertData3D<int16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertData3D<int32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertData3D<int64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertData3D<uint8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertData3D<uint16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertData3D<uint32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertData3D<uint64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

template <typename T>
float* convertMatrixData(uint8_t* input_data, const Dimension2D dims, const size_t num_bytes_for_one_vec)
{
    Matrix<T> x, y, z;
    x.setInternalData(reinterpret_cast<T*>(input_data), dims.rows, dims.cols);
    y.setInternalData(reinterpret_cast<T*>(&(input_data[num_bytes_for_one_vec])), dims.rows, dims.cols);
    z.setInternalData(reinterpret_cast<T*>(&(input_data[2 * num_bytes_for_one_vec])), dims.rows, dims.cols);

    float* output_data = new float[(dims.rows - 1) * (dims.cols - 1) * 4 * 3];
    size_t idx = 0;
    for (size_t r = 0; r < (dims.rows - 1); r++)
    {
        for (size_t c = 0; c < (dims.cols - 1); c++)
        {
            const size_t idx0_x = idx;
            const size_t idx0_y = idx + 1;
            const size_t idx0_z = idx + 2;

            const size_t idx1_x = idx + 3;
            const size_t idx1_y = idx + 4;
            const size_t idx1_z = idx + 5;

            const size_t idx2_x = idx + 6;
            const size_t idx2_y = idx + 7;
            const size_t idx2_z = idx + 8;

            const size_t idx3_x = idx + 9;
            const size_t idx3_y = idx + 10;
            const size_t idx3_z = idx + 11;
            idx = idx + 12;

            output_data[idx0_x] = x(r, c);
            output_data[idx1_x] = x(r + 1, c);
            output_data[idx2_x] = x(r + 1, c + 1);
            output_data[idx3_x] = x(r, c + 1);

            output_data[idx0_y] = y(r, c);
            output_data[idx1_y] = y(r + 1, c);
            output_data[idx2_y] = y(r + 1, c + 1);
            output_data[idx3_y] = y(r, c + 1);

            output_data[idx0_z] = z(r, c);
            output_data[idx1_z] = z(r + 1, c);
            output_data[idx2_z] = z(r + 1, c + 1);
            output_data[idx3_z] = z(r, c + 1);
        }
    }

    x.setInternalData(nullptr, 0, 0);
    y.setInternalData(nullptr, 0, 0);
    z.setInternalData(nullptr, 0, 0);

    return output_data;
}

inline float* convertMatrixDataOuter(uint8_t* input_data,
                                     const DataType data_type,
                                     const Dimension2D dims,
                                     const size_t num_bytes_for_one_vec)
{
    float* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertMatrixData<float>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertMatrixData<double>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertMatrixData<int8_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertMatrixData<int16_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertMatrixData<int32_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertMatrixData<int64_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertMatrixData<uint8_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertMatrixData<uint16_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertMatrixData<uint32_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertMatrixData<uint64_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

template <typename T>
inline float* convertVerticesData(uint8_t* input_data,
                                  const uint32_t num_vertices,
                                  const uint32_t num_indices,
                                  const uint32_t num_bytes_per_element)
{
    float* output_data = new float[num_indices * 3 * 3];
    Vector<Point3D<T>> vertices;
    Vector<IndexTriplet> indices;

    vertices.setInternalData(reinterpret_cast<Point3D<T>*>(input_data), num_vertices);
    indices.setInternalData(reinterpret_cast<IndexTriplet*>(&(input_data[num_vertices * num_bytes_per_element * 3])),
                            num_indices);

    size_t idx = 0;

    for (size_t k = 0; k < num_indices; k++)
    {
        const Point3D<T> p0 = vertices(indices(k).i0);
        const Point3D<T> p1 = vertices(indices(k).i1);
        const Point3D<T> p2 = vertices(indices(k).i2);

        output_data[idx] = p0.x;
        output_data[idx + 1] = p0.y;
        output_data[idx + 2] = p0.z;

        output_data[idx + 3] = p1.x;
        output_data[idx + 4] = p1.y;
        output_data[idx + 5] = p1.z;

        output_data[idx + 6] = p2.x;
        output_data[idx + 7] = p2.y;
        output_data[idx + 8] = p2.z;
        idx += 9;
    }

    vertices.setInternalData(nullptr, 0);
    indices.setInternalData(nullptr, 0);

    return output_data;
}

inline float* convertVerticesDataOuter(uint8_t* input_data,
                                       const DataType data_type,
                                       const uint32_t num_vertices,
                                       const uint32_t num_indices,
                                       const uint32_t num_bytes_per_element)
{
    float* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertVerticesData<float>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertVerticesData<double>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertVerticesData<int8_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertVerticesData<int16_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertVerticesData<int32_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertVerticesData<int64_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertVerticesData<uint8_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertVerticesData<uint16_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertVerticesData<uint32_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertVerticesData<uint64_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

template <typename T>
inline float* convertSingleMatrixDataInner(uint8_t* const data_buffer,
                                           const size_t num_rows,
                                           const size_t num_cols,
                                           const Vec2D<double>& tile_size)
{
    float* output_data = new float[num_rows * num_cols * 4 * 3];
    Matrix<float> z;

    z.setInternalData(reinterpret_cast<float*>(data_buffer), num_rows, num_cols);

    size_t idx = 0;
    for (size_t r = 0; r < num_rows; r++)
    {
        for (size_t c = 0; c < num_cols; c++)
        {
            const float x = static_cast<float>(c) * tile_size.x;
            const float y = static_cast<float>(r) * tile_size.y;

            const size_t idx0_x = idx;
            const size_t idx0_y = idx + 1;
            const size_t idx0_z = idx + 2;

            const size_t idx1_x = idx + 3;
            const size_t idx1_y = idx + 4;
            const size_t idx1_z = idx + 5;

            const size_t idx2_x = idx + 6;
            const size_t idx2_y = idx + 7;
            const size_t idx2_z = idx + 8;

            const size_t idx3_x = idx + 9;
            const size_t idx3_y = idx + 10;
            const size_t idx3_z = idx + 11;
            idx = idx + 12;

            output_data[idx0_x] = x;
            output_data[idx1_x] = x;
            output_data[idx2_x] = x + tile_size.x;
            output_data[idx3_x] = x + tile_size.x;

            output_data[idx0_y] = y;
            output_data[idx1_y] = y + tile_size.y;
            output_data[idx2_y] = y + tile_size.y;
            output_data[idx3_y] = y;

            output_data[idx0_z] = z(r, c);
            output_data[idx1_z] = z(r, c);
            output_data[idx2_z] = z(r, c);
            output_data[idx3_z] = z(r, c);
        }
    }

    z.setInternalData(nullptr, 0, 0);

    return output_data;
}

inline float* convertSingleMatrixDataOuter(uint8_t* input_data,
                                           const DataType data_type,
                                           const Dimension2D dims,
                                           const Vec2D<double>& tile_size)
{
    float* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertSingleMatrixDataInner<float>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertSingleMatrixDataInner<double>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertSingleMatrixDataInner<int8_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertSingleMatrixDataInner<int16_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertSingleMatrixDataInner<int32_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertSingleMatrixDataInner<int64_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertSingleMatrixDataInner<uint8_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertSingleMatrixDataInner<uint16_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertSingleMatrixDataInner<uint32_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertSingleMatrixDataInner<uint64_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

template <typename T> inline float* convertTrianglesDataInner(uint8_t* input_data, const uint32_t num_elements)
{
    float* output_data = new float[num_elements * 3 * 3];
    Vector<Triangle3D<T>> vertices;

    vertices.setInternalData(reinterpret_cast<Triangle3D<T>*>(input_data), num_elements);

    size_t idx = 0;

    for (size_t k = 0; k < num_elements; k++)
    {
        const Triangle3D<T> tri = vertices(k);

        output_data[idx] = tri.p0.x;
        output_data[idx + 1] = tri.p0.y;
        output_data[idx + 2] = tri.p0.z;

        output_data[idx + 3] = tri.p1.x;
        output_data[idx + 4] = tri.p1.y;
        output_data[idx + 5] = tri.p1.z;

        output_data[idx + 6] = tri.p2.x;
        output_data[idx + 7] = tri.p2.y;
        output_data[idx + 8] = tri.p2.z;
        idx += 9;
    }

    vertices.setInternalData(nullptr, 0);

    return output_data;
}

inline float* convertTrianglesData(uint8_t* input_data, const DataType data_type, const uint32_t num_elements)
{
    float* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertTrianglesDataInner<float>(input_data, num_elements);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertTrianglesDataInner<double>(input_data, num_elements);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertTrianglesDataInner<int8_t>(input_data, num_elements);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertTrianglesDataInner<int16_t>(input_data, num_elements);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertTrianglesDataInner<int32_t>(input_data, num_elements);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertTrianglesDataInner<int64_t>(input_data, num_elements);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertTrianglesDataInner<uint8_t>(input_data, num_elements);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertTrianglesDataInner<uint16_t>(input_data, num_elements);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertTrianglesDataInner<uint32_t>(input_data, num_elements);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertTrianglesDataInner<uint64_t>(input_data, num_elements);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

template <typename T>
uint8_t* convertStairs(const uint8_t* const input_data,
                       const size_t num_elements,
                       const size_t num_bytes_per_element,
                       const size_t num_bytes_for_one_vec)
{
    const size_t num_support_points = num_elements - 1;

    uint8_t* output_data = new uint8_t[sizeof(float) * 2 * (num_elements + num_support_points)];

    float f0, f0_1, f1;
    uint8_t* const f0_data = reinterpret_cast<uint8_t*>(&f0);
    uint8_t* const f0_1_data = reinterpret_cast<uint8_t*>(&f0_1);
    uint8_t* const f1_data = reinterpret_cast<uint8_t*>(&f1);
    T t0, t0_1, t1;
    uint8_t* const t0_data = reinterpret_cast<uint8_t*>(&t0);
    uint8_t* const t0_1_data = reinterpret_cast<uint8_t*>(&t0_1);
    uint8_t* const t1_data = reinterpret_cast<uint8_t*>(&t1);

    size_t idx_x = 0;
    size_t idx_y = sizeof(float);

    for (size_t k = 0; k < (num_elements - 1); k++)
    {
        const size_t idx_0 = k * num_bytes_per_element;
        const size_t idx_0_1 = (k + 1) * num_bytes_per_element;
        const size_t idx_1 = num_bytes_for_one_vec + k * num_bytes_per_element;
        const uint8_t* const tmp_ptr_0 = &(input_data[idx_0]);
        const uint8_t* const tmp_ptr_0_1 = &(input_data[idx_0_1]);
        const uint8_t* const tmp_ptr_1 = &(input_data[idx_1]);

        for (size_t i = 0; i < num_bytes_per_element; i++)
        {
            t0_data[i] = tmp_ptr_0[i];
            t0_1_data[i] = tmp_ptr_0_1[i];
            t1_data[i] = tmp_ptr_1[i];
        }

        f0 = t0;
        f0_1 = t0_1;
        f1 = t1;

        for (size_t i = 0; i < sizeof(float); i++)
        {
            output_data[idx_x + i] = f0_data[i];
            output_data[idx_y + i] = f1_data[i];
        }
        idx_x += sizeof(float) * 2;
        idx_y += sizeof(float) * 2;

        for (size_t i = 0; i < sizeof(float); i++)
        {
            output_data[idx_x + i] = f0_1_data[i];
            output_data[idx_y + i] = f1_data[i];
        }
        idx_x += sizeof(float) * 2;
        idx_y += sizeof(float) * 2;
    }

    const size_t last_idx = num_elements - 1;

    const size_t idx_0 = last_idx * num_bytes_per_element;
    const size_t idx_1 = num_bytes_for_one_vec + last_idx * num_bytes_per_element;
    const uint8_t* const tmp_ptr_0 = &(input_data[idx_0]);
    const uint8_t* const tmp_ptr_1 = &(input_data[idx_1]);

    for (size_t i = 0; i < num_bytes_per_element; i++)
    {
        t0_data[i] = tmp_ptr_0[i];
        t1_data[i] = tmp_ptr_1[i];
    }

    f0 = t0;
    f1 = t1;

    for (size_t i = 0; i < sizeof(float); i++)
    {
        output_data[idx_x + i] = f0_data[i];
        output_data[idx_y + i] = f1_data[i];
    }
    idx_x += sizeof(float) * 2;
    idx_y += sizeof(float) * 2;

    return output_data;
}

inline uint8_t* convertStairsOuter(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec)
{
    uint8_t* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertStairs<float>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertStairs<double>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertStairs<int8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertStairs<int16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertStairs<int32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertStairs<int64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertStairs<uint8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertStairs<uint16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertStairs<uint32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertStairs<uint64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

#endif
