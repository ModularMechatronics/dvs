#include "main_application/plot_objects/draw_mesh/draw_mesh.h"

#include "outer_converter.h"

namespace
{
struct ConvertedData : ConvertedDataBase
{
    float* points_ptr;
    float* normals_ptr;
    float* mean_height_ptr;

    ConvertedData() : points_ptr{nullptr}, normals_ptr{nullptr}, mean_height_ptr{nullptr} {}

    ConvertedData(const ConvertedData& other) = delete;
    ConvertedData& operator=(const ConvertedData& other) = delete;
    ConvertedData(ConvertedData&& other) = delete;
    ConvertedData& operator=(ConvertedData&& other) = delete;

    ~ConvertedData() override
    {
        delete[] points_ptr;
        delete[] normals_ptr;
        delete[] mean_height_ptr;
    }
};

struct InputParams
{
    uint32_t num_vertices;
    uint32_t num_indices;
    uint32_t num_bytes_per_element;

    InputParams() = default;
    InputParams(const uint32_t num_vertices_, const uint32_t num_indices_, const uint32_t num_bytes_per_element_)
        : num_vertices{num_vertices_}, num_indices{num_indices_}, num_bytes_per_element{num_bytes_per_element_}
    {
    }
};

template <typename T>
std::unique_ptr<ConvertedData> convertData(const uint8_t* const input_data, const InputParams& input_params);
template <typename T>
std::unique_ptr<ConvertedData> convertDataSeparateVectors(const uint8_t* const input_data,
                                                          const InputParams& input_params);

struct Converter
{
    template <class T>
    std::unique_ptr<ConvertedData> convert(const uint8_t* const input_data, const InputParams& input_params) const
    {
        return convertData<T>(input_data, input_params);
    }
};

struct ConverterSeparateVectors
{
    template <class T>
    std::unique_ptr<ConvertedData> convert(const uint8_t* const input_data, const InputParams& input_params) const
    {
        return convertDataSeparateVectors<T>(input_data, input_params);
    }
};

}  // namespace

DrawMesh::DrawMesh(const CommunicationHeader& hdr,
                   ReceivedData& received_data,
                   std::unique_ptr<const ConvertedDataBase>& converted_data,
                   const PlotObjectAttributes& plot_object_attributes,
                   const PropertiesData& properties_data,
                   const ShaderCollection& shader_collection,
                   ColorPicker& color_picker)
    : PlotObjectBase(received_data, hdr, plot_object_attributes, properties_data, shader_collection, color_picker),
      vertex_buffer_{OGLPrimitiveType::TRIANGLES},
      converted_data_{std::move(converted_data)}
{
    if ((function_ != Function::DRAW_MESH) && (function_ != Function::DRAW_MESH_SEPARATE_VECTORS))
    {
        throw std::runtime_error("Invalid function type for DrawMesh!");
    }

    const ConvertedData* const converted_data_local = static_cast<const ConvertedData* const>(converted_data_.get());

    num_indices_ = hdr.get(CommunicationHeaderObjectType::NUM_INDICES).as<uint32_t>();

    points_ptr_ = converted_data_local->points_ptr;

    interpolate_colormap_ = true;

    num_elements_to_render_ = num_indices_ * 3;

    vertex_buffer_.addBuffer(points_ptr_, num_elements_to_render_, 3);
    vertex_buffer_.addBuffer(converted_data_local->normals_ptr, num_elements_to_render_, 3);
    vertex_buffer_.addBuffer(converted_data_local->mean_height_ptr, num_elements_to_render_, 1);
}

void DrawMesh::findMinMax()
{
    min_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    max_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    size_t idx = 0;
    for (size_t k = 0; k < (num_indices_ * 3 * 3); k += 3)
    {
        const Point3d current_point(points_ptr_[k], points_ptr_[k + 1], points_ptr_[k + 2]);
        min_vec.x = std::min(current_point.x, min_vec.x);
        min_vec.y = std::min(current_point.y, min_vec.y);
        min_vec.z = std::min(current_point.z, min_vec.z);

        max_vec.x = std::max(current_point.x, max_vec.x);
        max_vec.y = std::max(current_point.y, max_vec.y);
        max_vec.z = std::max(current_point.z, max_vec.z);

        idx += 3;
    }
}

void DrawMesh::render()
{
    glEnable(GL_BLEND);

    shader_collection_.draw_mesh_shader.use();

    preRender(&shader_collection_.draw_mesh_shader);

    shader_collection_.draw_mesh_shader.uniform_handles.edge_color.setColor(edge_color_);
    shader_collection_.draw_mesh_shader.uniform_handles.face_color.setColor(face_color_);
    shader_collection_.draw_mesh_shader.base_uniform_handles.min_z.setFloat(min_vec.z);
    shader_collection_.draw_mesh_shader.base_uniform_handles.max_z.setFloat(max_vec.z);
    shader_collection_.draw_mesh_shader.base_uniform_handles.alpha.setFloat(alpha_);

    if (has_color_map_)
    {
        glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "color_map_selection"),
                    static_cast<int>(color_map_) + 1);
        glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "interpolate_colormap"),
                    static_cast<int>(interpolate_colormap_));
    }
    else
    {
        glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "color_map_selection"), 0);
    }

    shader_collection_.draw_mesh_shader.uniform_handles.is_edge.setInt(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (has_edge_color_)
    {
        vertex_buffer_.render(num_elements_to_render_);
    }

    shader_collection_.draw_mesh_shader.uniform_handles.is_edge.setInt(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (has_face_color_)
    {
        vertex_buffer_.render(num_elements_to_render_);
    }

    glDisable(GL_BLEND);
}

LegendProperties DrawMesh::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::POLYGON;
    lp.edge_color = edge_color_;

    if (has_color_map_)
    {
        lp.has_color_map = true;
        lp.color_map = color_map_;
    }
    else
    {
        lp.has_color_map = false;
        lp.face_color = face_color_;
    }

    return lp;
}

DrawMesh::~DrawMesh() {}

std::unique_ptr<const ConvertedDataBase> DrawMesh::convertRawData(const CommunicationHeader& hdr,
                                                                  const PlotObjectAttributes& attributes,
                                                                  const PropertiesData& properties_data,
                                                                  const uint8_t* const data_ptr)
{
    const InputParams input_params(attributes.num_vertices, attributes.num_indices, attributes.num_bytes_per_element);

    if (attributes.function == Function::DRAW_MESH)
    {
        std::unique_ptr<const ConvertedDataBase> converted_data_base{
            applyConverter<ConvertedData>(data_ptr, attributes.data_type, Converter{}, input_params)};
        return converted_data_base;
    }
    else
    {
        std::unique_ptr<const ConvertedDataBase> converted_data_base{
            applyConverter<ConvertedData>(data_ptr, attributes.data_type, ConverterSeparateVectors{}, input_params)};
        return converted_data_base;
    }
}

namespace
{

template <typename T>
std::unique_ptr<ConvertedData> convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    ConvertedData* converted_data = new ConvertedData;
    converted_data->points_ptr = new float[input_params.num_indices * 3 * 3];
    converted_data->normals_ptr = new float[input_params.num_indices * 3 * 3];
    converted_data->mean_height_ptr = new float[input_params.num_indices * 3];

    VectorConstView<Point3<T>> vertices{reinterpret_cast<const Point3<T>*>(input_data), input_params.num_vertices};
    VectorConstView<IndexTriplet> indices{
        reinterpret_cast<const IndexTriplet*>(
            &(input_data[input_params.num_vertices * input_params.num_bytes_per_element * 3])),
        input_params.num_indices};

    size_t idx = 0, height_idx = 0;

    for (size_t k = 0; k < input_params.num_indices; k++)
    {
        const Point3<T> p0 = vertices(indices(k).i0);
        const Point3<T> p1 = vertices(indices(k).i1);
        const Point3<T> p2 = vertices(indices(k).i2);

        const Plane<T> plane{planeFromThreePoints<T>(p0, p1, p2)};
        const Vec3<T> normal_vec{plane.a, plane.b, plane.c};
        const Vec3<T> normalized_normal_vec{normal_vec.normalized()};

        converted_data->points_ptr[idx] = p0.x;
        converted_data->points_ptr[idx + 1] = p0.y;
        converted_data->points_ptr[idx + 2] = p0.z;

        converted_data->points_ptr[idx + 3] = p1.x;
        converted_data->points_ptr[idx + 4] = p1.y;
        converted_data->points_ptr[idx + 5] = p1.z;

        converted_data->points_ptr[idx + 6] = p2.x;
        converted_data->points_ptr[idx + 7] = p2.y;
        converted_data->points_ptr[idx + 8] = p2.z;

        converted_data->normals_ptr[idx] = normalized_normal_vec.x;
        converted_data->normals_ptr[idx + 1] = normalized_normal_vec.y;
        converted_data->normals_ptr[idx + 2] = normalized_normal_vec.z;

        converted_data->normals_ptr[idx + 3] = normalized_normal_vec.x;
        converted_data->normals_ptr[idx + 4] = normalized_normal_vec.y;
        converted_data->normals_ptr[idx + 5] = normalized_normal_vec.z;

        converted_data->normals_ptr[idx + 6] = normalized_normal_vec.x;
        converted_data->normals_ptr[idx + 7] = normalized_normal_vec.y;
        converted_data->normals_ptr[idx + 8] = normalized_normal_vec.z;

        const float z_m = (p0.z + p1.z + p2.z) * 0.33333333333f;
        converted_data->mean_height_ptr[height_idx] = z_m;
        converted_data->mean_height_ptr[height_idx + 1] = z_m;
        converted_data->mean_height_ptr[height_idx + 2] = z_m;

        idx += 9;
        height_idx += 3;
    }

    return std::unique_ptr<ConvertedData>{converted_data};
}

template <typename T>
std::unique_ptr<ConvertedData> convertDataSeparateVectors(const uint8_t* const input_data,
                                                          const InputParams& input_params)
{
    ConvertedData* converted_data = new ConvertedData;

    converted_data->points_ptr = new float[input_params.num_indices * 3 * 3];
    converted_data->normals_ptr = new float[input_params.num_indices * 3 * 3];
    converted_data->mean_height_ptr = new float[input_params.num_indices * 3];

    VectorConstView<T> x{reinterpret_cast<const T*>(input_data), input_params.num_vertices};
    VectorConstView<T> y{
        reinterpret_cast<const T*>(&(input_data[input_params.num_vertices * input_params.num_bytes_per_element])),
        input_params.num_vertices};
    VectorConstView<T> z{
        reinterpret_cast<const T*>(&(input_data[input_params.num_vertices * input_params.num_bytes_per_element * 2])),
        input_params.num_vertices};

    VectorConstView<IndexTriplet> indices{
        reinterpret_cast<const IndexTriplet*>(
            &(input_data[input_params.num_vertices * input_params.num_bytes_per_element * 3])),
        input_params.num_indices};

    size_t idx = 0, height_idx = 0;

    for (size_t k = 0; k < input_params.num_indices; k++)
    {
        const Point3<T> p0{x(indices(k).i0), y(indices(k).i0), z(indices(k).i0)};
        const Point3<T> p1{x(indices(k).i1), y(indices(k).i1), z(indices(k).i1)};
        const Point3<T> p2{x(indices(k).i2), y(indices(k).i2), z(indices(k).i2)};

        const Plane<T> plane{planeFromThreePoints<T>(p0, p1, p2)};
        const Vec3<T> normal_vec{plane.a, plane.b, plane.c};
        const Vec3<T> normalized_normal_vec{normal_vec.normalized()};

        converted_data->points_ptr[idx] = p0.x;
        converted_data->points_ptr[idx + 1] = p0.y;
        converted_data->points_ptr[idx + 2] = p0.z;

        converted_data->points_ptr[idx + 3] = p1.x;
        converted_data->points_ptr[idx + 4] = p1.y;
        converted_data->points_ptr[idx + 5] = p1.z;

        converted_data->points_ptr[idx + 6] = p2.x;
        converted_data->points_ptr[idx + 7] = p2.y;
        converted_data->points_ptr[idx + 8] = p2.z;

        converted_data->normals_ptr[idx] = normalized_normal_vec.x;
        converted_data->normals_ptr[idx + 1] = normalized_normal_vec.y;
        converted_data->normals_ptr[idx + 2] = normalized_normal_vec.z;

        converted_data->normals_ptr[idx + 3] = normalized_normal_vec.x;
        converted_data->normals_ptr[idx + 4] = normalized_normal_vec.y;
        converted_data->normals_ptr[idx + 5] = normalized_normal_vec.z;

        converted_data->normals_ptr[idx + 6] = normalized_normal_vec.x;
        converted_data->normals_ptr[idx + 7] = normalized_normal_vec.y;
        converted_data->normals_ptr[idx + 8] = normalized_normal_vec.z;

        const float z_m = (p0.z + p1.z + p2.z) * 0.33333333333f;
        converted_data->mean_height_ptr[height_idx] = z_m;
        converted_data->mean_height_ptr[height_idx + 1] = z_m;
        converted_data->mean_height_ptr[height_idx + 2] = z_m;

        idx += 9;
        height_idx += 3;
    }

    return std::unique_ptr<ConvertedData>{converted_data};
}

}  // namespace
