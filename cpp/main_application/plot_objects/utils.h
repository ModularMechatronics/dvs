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
        case Function::DRAW_TRIANGLES_3D:
            return 3;
        case Function::DRAW_MESH:
            return 3;
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

void fillBufferWithData(uint8_t* const points_ptr,
                        const uint8_t* const data_ptr,
                        const size_t num_elements,
                        const size_t num_bytes_per_element,
                        const size_t num_bytes_for_one_vec)
{
    
    size_t idx_x = 0;
    size_t idx_y = num_bytes_per_element;
    size_t idx_z = 2 * num_bytes_per_element;
    const size_t num_bytes_per_element_times_3 = num_bytes_per_element * 3;
    const size_t num_bytes_for_one_vec_times_2 = num_bytes_for_one_vec * 2;

    for(size_t k = 0; k < num_elements; k++)
    {
        const size_t idx_0 = k * num_bytes_per_element;
        const size_t idx_1 = num_bytes_for_one_vec + idx_0;
        const size_t idx_2 = num_bytes_for_one_vec_times_2 + idx_0;
        const uint8_t* const tmp_ptr_0 = &(data_ptr[idx_0]);
        const uint8_t* const tmp_ptr_1 = &(data_ptr[idx_1]);
        const uint8_t* const tmp_ptr_2 = &(data_ptr[idx_2]);

        for(size_t i = 0; i < num_bytes_per_element; i++)
        {
            points_ptr[idx_x + i] = tmp_ptr_0[i];
            points_ptr[idx_y + i] = tmp_ptr_1[i];
            points_ptr[idx_z + i] = tmp_ptr_2[i];
        }
        idx_x += num_bytes_per_element_times_3;
        idx_y += num_bytes_per_element_times_3;
        idx_z += num_bytes_per_element_times_3;
    }
}

inline std::pair<Vec3Dd, Vec3Dd> findMinMaxFromThreeVectors(uint8_t* const data_buffer,
                                                            const size_t num_elements,
                                                            const size_t num_bytes_for_one_vec,
                                                            const DataType data_type)
{
    Vec3Dd min_vec, max_vec;

    if(data_type == DataType::FLOAT)
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
    else if(data_type == DataType::DOUBLE)
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
    else if(data_type == DataType::INT8)
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
    else if(data_type == DataType::INT16)
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
    else if(data_type == DataType::INT32)
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
    else if(data_type == DataType::INT64)
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
    else if(data_type == DataType::UINT8)
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
    else if(data_type == DataType::UINT16)
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
    else if(data_type == DataType::UINT32)
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
    else if(data_type == DataType::UINT64)
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
    else if(data_type == DataType::UNKNOWN)
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

    if(data_type == DataType::FLOAT)
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
    else if(data_type == DataType::DOUBLE)
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
    else if(data_type == DataType::INT8)
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
    else if(data_type == DataType::INT16)
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
    else if(data_type == DataType::INT32)
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
    else if(data_type == DataType::INT64)
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
    else if(data_type == DataType::UINT8)
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
    else if(data_type == DataType::UINT16)
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
    else if(data_type == DataType::UINT32)
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
    else if(data_type == DataType::UINT64)
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
    else if(data_type == DataType::UNKNOWN)
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

    if(data_type == DataType::FLOAT)
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
    else if(data_type == DataType::DOUBLE)
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
    else if(data_type == DataType::INT8)
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
    else if(data_type == DataType::INT16)
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
    else if(data_type == DataType::INT32)
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
    else if(data_type == DataType::INT64)
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
    else if(data_type == DataType::UINT8)
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
    else if(data_type == DataType::UINT16)
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
    else if(data_type == DataType::UINT32)
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
    else if(data_type == DataType::UINT64)
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
    else if(data_type == DataType::UNKNOWN)
    {
        throw std::runtime_error("Unknown data type!");
    }
    else
    {
        throw std::runtime_error("Got default in data type switch!");
    }

    return std::pair<Vec3Dd, Vec3Dd>(min_vec, max_vec);
}

#endif
