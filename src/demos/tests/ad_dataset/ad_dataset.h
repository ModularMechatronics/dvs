#ifndef DEMOS_TESTS_AD_DATASET_H_
#define DEMOS_TESTS_AD_DATASET_H_

#include <Box2D/Box2D.h>
#include <Box2D/Particle/b2Particle.h>

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <tuple>

#include "dvs/dvs.h"

using namespace dvs;

namespace ad_dataset
{

struct PointCollection
{
    VectorConstView<float> x;
    VectorConstView<float> y;
    VectorConstView<float> z;
};

class DatasetReader
{
private:
    std::vector<std::vector<unsigned char>> lidar_raw_data_;
    std::vector<std::vector<unsigned char>> img_raw_data_;

    std::vector<uint32_t> num_points_;

    std::vector<std::string> lidar_file_paths_;
    std::vector<PointCollection> point_collections_;

    std::vector<std::string> img_file_paths_;
    std::vector<ImageRGB<uint8>> images_;

    void readLidarFile(const std::string& bin_path);
    void readCameraFile(const std::string& bin_path);

public:
    DatasetReader() = delete;
    DatasetReader(const std::string& dataset_root_path);

    PointCollection getPointCollection(const size_t idx)
    {
        return point_collections_[idx];
    }

    ImageRGBConstView<uint8> getImage(const size_t idx)
    {
        return images_[idx].constView();
    }

    size_t numLidarFiles() const
    {
        return point_collections_.size();
    }

    size_t numImgFiles() const
    {
        return img_raw_data_.size();
    }
};

void testBasic();

}  // namespace ad_dataset

#endif  // DEMOS_TESTS_AD_DATASET_H_
