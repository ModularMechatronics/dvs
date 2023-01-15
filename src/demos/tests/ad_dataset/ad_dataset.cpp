#include "ad_dataset.h"

#include <curses.h>

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

    PointCollection pc;

    const float* const x_ptr = reinterpret_cast<const float* const>(read_data.data() + sizeof(uint32_t));
    const float* const y_ptr =
        reinterpret_cast<const float* const>(read_data.data() + sizeof(uint32_t) + sizeof(float) * n_points);
    const float* const z_ptr =
        reinterpret_cast<const float* const>(read_data.data() + sizeof(uint32_t) + 2 * sizeof(float) * n_points);

    pc.x = VectorConstView<float>{x_ptr, n_points};
    pc.y = VectorConstView<float>{y_ptr, n_points};
    pc.z = VectorConstView<float>{z_ptr, n_points};

    point_collections_.push_back(pc);
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

    for (const std::string& lidar_file_path : lidar_file_paths_)
    {
        readBinaryFile(lidar_file_path);
    }

    std::cout << "Finished reading files!" << std::endl;
}

void testScroll()
{
    initscr();

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

    size_t idx = 0U;

    while (true)
    {
        const PointCollection& pc = dataset_reader.getPointCollection(idx);

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

        char q = getch();

        if (q == 'a')
        {
            idx = (idx == 0U) ? 0U : (idx - 1U);
        }
        else if (q == 'd')
        {
            idx = (idx == dataset_reader.numLidarFiles() - 1U) ? dataset_reader.numLidarFiles() - 1U : (idx + 1U);
        }
        else
        {
            break;
        }
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
