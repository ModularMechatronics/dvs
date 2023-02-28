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
    std::vector<PointCollection> point_collections_;
    std::vector<std::vector<unsigned char>> lidar_raw_data_;

    std::vector<ImageRGB<uint8>> images_front_;
    std::vector<ImageRGB<uint8>> images_left_;
    std::vector<ImageRGB<uint8>> images_right_;

    static constexpr int kMaxReadFrame = 197;

    void readLidarFile(const std::string& bin_path);

    void readCamera(const std::string& folder_path, std::vector<ImageRGB<uint8_t>>& destination);
    void readCameraFile(const std::string& bin_path, std::vector<ImageRGB<uint8_t>>& destination);

public:
    DatasetReader() = delete;
    DatasetReader(const std::string& dataset_root_path);

    PointCollection getPointCollection(const size_t idx)
    {
        return point_collections_[idx];
    }

    ImageRGBConstView<uint8> getLeftImage(const size_t idx)
    {
        return images_left_[idx].constView();
    }

    ImageRGBConstView<uint8> getRightImage(const size_t idx)
    {
        return images_right_[idx].constView();
    }

    ImageRGBConstView<uint8> getFrontImage(const size_t idx)
    {
        return images_front_[idx].constView();
    }

    size_t numLidarFiles() const
    {
        return point_collections_.size();
    }

    size_t numImgFiles() const
    {
        return images_front_.size();
    }

    void saveToBigBlob(const std::string& file_name) const;
};

void testBasic();

}  // namespace ad_dataset

#endif  // DEMOS_TESTS_AD_DATASET_H_
