#include "tests/joints/joints.h"

#include "dvs/dvs.h"

using namespace dvs;

namespace joints
{

ImageRGBA<uint8_t> readShapeImage(const std::string bin_path)
{
    
    std::ifstream input(bin_path, std::ios::binary);

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    std::uint16_t num_rows, num_cols;
    buffer.data();

    std::memcpy(&num_cols, buffer.data(), sizeof(std::uint16_t));
    std::memcpy(&num_rows, buffer.data() + sizeof(std::uint16_t), sizeof(std::uint16_t));

    const std::uint8_t* const img_raw_ptr = buffer.data() + 2 * sizeof(std::uint16_t);

    ImageRGBA<uint8_t> output_img{num_rows, num_cols};

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
    
    return output_img;
}


void testBasic()
{
    std::vector<std::string> file_names = {
        "circle.bin",
        "damper.bin",
        "pentagon.bin",
        "rounded_square.bin",
        "spring.bin",
        "square.bin",
        "star.bin"
    };

    const std::string bin_path = "../demos/tests/joints/images/";

    const ImageRGBA<uint8_t> read_img = readShapeImage(bin_path + "spring.bin");

    const std::string project_file_path = "../../project_files/joints.dvs";

    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();

    axis({-32.0, -32.0, -10.0}, {500, 500, 10.0});
    view(0, 90);

    imShow(read_img, properties::ZOffset(-0.25f));

}

}
