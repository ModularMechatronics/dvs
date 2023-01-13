#include "ad_dataset.h"

#include <algorithm>
#include <filesystem>

namespace ad_dataset
{

void DatasetReader::readBinaryFile(const std::string& bin_path)
{
    std::ifstream input(bin_path, std::ios::binary);

    lidar_raw_data_.push_back(std::vector<unsigned char>(std::istreambuf_iterator<char>(input), {}));

    std::vector<unsigned char>& read_data = lidar_raw_data_.back();

    uint32_t n_points;
    std::memcpy(&n_points, read_data.data(), sizeof(uint32_t));

    num_points_.push_back(n_points);

    point_collections_.emplace_back(n_points);

    PointCollection& pc = point_collections_.back();

    std::memcpy(pc.x.data(), read_data.data() + sizeof(uint32_t), sizeof(float) * n_points);
    std::memcpy(pc.y.data(), read_data.data() + sizeof(uint32_t) + sizeof(float) * n_points, sizeof(float) * n_points);
    std::memcpy(
        pc.z.data(), read_data.data() + sizeof(uint32_t) + 2 * sizeof(float) * n_points, sizeof(float) * n_points);
}

DatasetReader::DatasetReader(const std::string& dataset_root_path)
{
    const std::string ds_path = dataset_root_path.back() == '/' ? dataset_root_path : dataset_root_path + "/";

    const std::string lidar_path = ds_path + "lidar/";

    const std::string lidar_file = lidar_path + "00000189.bin";

    for (const auto& entry : std::filesystem::directory_iterator(lidar_path))
    {
        lidar_file_paths_.push_back(entry.path());
    }

    std::sort(lidar_file_paths_.begin(), lidar_file_paths_.end());

    const size_t num_files_to_read = 1U;
    size_t idx = 0U;
    for (const std::string& lidar_file_path : lidar_file_paths_)
    {
        /*if (idx >= num_files_to_read)
        {
            break;
        }*/
        readBinaryFile(lidar_file_path);

        idx++;
    }
}

void testBasic()
{
    DatasetReader dataset_reader{"/Users/danielpi/work/dvs/leddar_dataset/20200706_171559_part27_1170_1370/output"};

    setCurrentElement("secondary");
    clearView();
    waitForFlush();
    axis({-20.0, -20.0, -20.0}, {20.0, 20.0, 20.0});
    view(-38.0, 32.0);

    setCurrentElement("main");
    clearView();
    waitForFlush();
    axis({-20.0, -20.0, -20.0}, {20.0, 20.0, 20.0});
    view(-38.0, 32.0);

    for (size_t k = 0; k < dataset_reader.numLidarFiles(); k++)
    {
        const PointCollection& pc = dataset_reader.getPointCollection(k);

        setCurrentElement("main");
        softClearView();
        scatter3(pc.x,
                 pc.y,
                 pc.z,
                 properties::DistanceFrom::xyz({0, 0, 0}, 0.0, 64.0),
                 properties::ColorMap::Viridis(),
                 properties::PointSize(5),
                 properties::ScatterStyle::Disc());

        setCurrentElement("secondary");
        softClearView();
        scatter3(pc.x,
                 pc.y,
                 pc.z,
                 properties::DistanceFrom::xyz({0, 0, 0}, 0.0, 64.0),
                 properties::ColorMap::Viridis(),
                 properties::PointSize(5),
                 properties::ScatterStyle::Disc());

        flushMultipleElements("main", "secondary");
    }
}

}  // namespace ad_dataset
