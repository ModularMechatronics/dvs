#ifndef DEMOS_TESTS_JOINTS_JOINTS_H_
#define DEMOS_TESTS_JOINTS_JOINTS_H_

#include "dvs/dvs.h"

using namespace dvs;

namespace joints
{

class Joints
{
private:

public:
    Joints();
    ~Joints();
};

class ShapeImages
{
private:
    void readShapeImage(const std::string bin_path, ImageRGBA<uint8_t>& output_img)
    {
        
        std::ifstream input(bin_path, std::ios::binary);

        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

        std::uint16_t num_rows, num_cols;
        buffer.data();

        std::memcpy(&num_cols, buffer.data(), sizeof(std::uint16_t));
        std::memcpy(&num_rows, buffer.data() + sizeof(std::uint16_t), sizeof(std::uint16_t));

        const std::uint8_t* const img_raw_ptr = buffer.data() + 2 * sizeof(std::uint16_t);

        output_img.resize(num_rows, num_cols);

        const size_t num_element_per_channel = num_rows * num_cols;

        for (size_t ch = 0; ch < 3; ch++)
        {
            for (size_t r = 0; r < num_rows; r++)
            {
                for (size_t c = 0; c < num_cols; c++)
                {
                    const std::uint8_t pixel_val = img_raw_ptr[ch * num_element_per_channel + r * num_cols + c];

                    size_t mapped_ch = 0;
                    switch(ch)
                    {
                        case 0:
                            mapped_ch = 2;
                            break;
                        case 1:
                            mapped_ch = 1;
                            break;
                        case 2:
                            mapped_ch = 0;
                            break;
                        default:
                            mapped_ch = 0;
                    }
                    output_img(r, c, mapped_ch) = pixel_val;
                }
            }
        }

        for (size_t r = 0; r < num_rows; r++)
        {
            for (size_t c = 0; c < num_cols; c++)
            {
                // If white, set alpha to 0
                const bool cond = (output_img(r, c, 0) == 255) &&
                    (output_img(r, c, 1) == 255) &&
                    (output_img(r, c, 2) == 255);
                if(cond)
                {
                    output_img(r, c, 3) = 0;
                }
                else
                {
                    output_img(r, c, 3) = 255;
                }
            }
        }
    }

    ImageRGBA<uint8_t> circle_;
    ImageRGBA<uint8_t> damper_;
    ImageRGBA<uint8_t> pentagon_;
    ImageRGBA<uint8_t> rounded_square_;
    ImageRGBA<uint8_t> spring_;
    ImageRGBA<uint8_t> square_;
    ImageRGBA<uint8_t> star_;

public:
    ShapeImages() : circle_{},
        damper_{},
        pentagon_{},
        rounded_square_{},
        spring_{},
        square_{},
        star_{}
    {
        const std::string bin_path = "../demos/tests/joints/images/";

        readShapeImage(bin_path + "circle.bin", circle_);
        readShapeImage(bin_path + "damper.bin", damper_);
        readShapeImage(bin_path + "pentagon.bin", pentagon_);
        readShapeImage(bin_path + "rounded_square.bin", rounded_square_);
        readShapeImage(bin_path + "spring.bin", spring_);
        readShapeImage(bin_path + "square.bin", square_);
        readShapeImage(bin_path + "star.bin", star_);
    }

    ImageRGBAConstView<uint8_t> getCircle()
    {
        return circle_.constView();
    }

    ImageRGBAConstView<uint8_t> getDamper()
    {
        return damper_.constView();
    }

    ImageRGBAConstView<uint8_t> getPentagon()
    {
        return pentagon_.constView();
    }

    ImageRGBAConstView<uint8_t> getRoundedSquare()
    {
        return rounded_square_.constView();
    }

    ImageRGBAConstView<uint8_t> getSpring()
    {
        return spring_.constView();
    }

    ImageRGBAConstView<uint8_t> getSquare()
    {
        return square_.constView();
    }

    ImageRGBAConstView<uint8_t> getStar()
    {
        return star_.constView();
    }

    
};

void testBasic();



}



#endif // DEMOS_TESTS_JOINTS_JOINTS_H_