#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "dvs/dvs.h"

using namespace dvs;
using namespace dvs::internal;

namespace prp = dvs::properties;

class TestTransmissionHeader : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

template <typename... Us> void variadicTemplateFunction(TransmissionHeader& hdr, const Us&... settings)
{
    hdr.extend(settings...);
}

TEST_F(TestTransmissionHeader, TestBasic0)
{
    TransmissionHeader hdr;
    hdr.append(TransmissionHeaderObjectType::FUNCTION, Function::PLOT2);
    hdr.append(TransmissionHeaderObjectType::DATA_STRUCTURE, DataStructure::VECTOR);
    hdr.append(TransmissionHeaderObjectType::DATA_TYPE, typeToDataTypeEnum<double>());
    hdr.append(TransmissionHeaderObjectType::NUM_BUFFERS_REQUIRED, toUInt8(2));

    variadicTemplateFunction(hdr, prp::Color(14, 56, 72), prp::Alpha(187), prp::LineWidth(4), prp::PERSISTENT);

    ASSERT_TRUE(hdr.hasType(TransmissionHeaderObjectType::FUNCTION));
    ASSERT_TRUE(hdr.hasType(TransmissionHeaderObjectType::DATA_STRUCTURE));
    ASSERT_TRUE(hdr.hasType(TransmissionHeaderObjectType::DATA_TYPE));
    ASSERT_TRUE(hdr.hasType(TransmissionHeaderObjectType::NUM_BUFFERS_REQUIRED));
    ASSERT_TRUE(hdr.hasType(TransmissionHeaderObjectType::PROPERTY));

    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::UNKNOWN));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::NUM_BYTES));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::BYTES_PER_ELEMENT));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::NUM_ELEMENTS));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::DIMENSION_2D));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::HAS_PAYLOAD));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::AZIMUTH));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::ELEVATION));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::NUM_AXES));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::AXIS_MIN_MAX_VEC));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::POS2D));
    ASSERT_FALSE(hdr.hasType(TransmissionHeaderObjectType::FIGURE_NUM));
}

TEST_F(TestTransmissionHeader, TestBasic)
{
    TransmissionHeader hdr;
    hdr.append(TransmissionHeaderObjectType::FUNCTION, Function::PLOT2);
    hdr.append(TransmissionHeaderObjectType::DATA_STRUCTURE, DataStructure::VECTOR);
    hdr.append(TransmissionHeaderObjectType::DATA_TYPE, typeToDataTypeEnum<double>());
    hdr.append(TransmissionHeaderObjectType::NUM_BUFFERS_REQUIRED, toUInt8(2));

    const prp::Color col(14, 56, 72);
    const prp::Alpha alpha(187);
    const prp::LineWidth lw(4);
    const prp::PointSize ps(63);

    variadicTemplateFunction(hdr, col, alpha, lw, prp::PERSISTENT, ps);

    const size_t num_bytes_required = hdr.totalNumBytesFromBuffer();

    uint8_t* const data_bytes = new uint8_t[num_bytes_required];
    hdr.fillBufferWithData(data_bytes);

    const TransmissionHeader received_hdr(data_bytes);

    const std::vector<TransmissionHeaderObject> values = received_hdr.getValues();

    for (size_t k = 0; k < values.size(); k++)
    {
        switch (values[k].type)
        {
            case TransmissionHeaderObjectType::FUNCTION:
                ASSERT_EQ(values[k].as<Function>(), Function::PLOT2);
                break;
            case TransmissionHeaderObjectType::DATA_STRUCTURE:
                ASSERT_EQ(values[k].as<DataStructure>(), DataStructure::VECTOR);
                break;
            case TransmissionHeaderObjectType::DATA_TYPE:
                ASSERT_EQ(values[k].as<DataType>(), DataType::DOUBLE);
                break;
            case TransmissionHeaderObjectType::NUM_BUFFERS_REQUIRED:
                ASSERT_EQ(values[k].as<uint8_t>(), static_cast<uint8_t>(2));
                break;
            case TransmissionHeaderObjectType::PROPERTY:
                switch (values[k].as<PropertyBase>().getPropertyType())
                {
                    case dvs::internal::PropertyType::COLOR:
                        ASSERT_EQ(values[k].as<prp::Color>().red, col.red);
                        ASSERT_EQ(values[k].as<prp::Color>().green, col.green);
                        ASSERT_EQ(values[k].as<prp::Color>().blue, col.blue);
                        break;
                    case dvs::internal::PropertyType::ALPHA:
                        ASSERT_EQ(values[k].as<prp::Alpha>().data, alpha.data);
                        break;
                    case dvs::internal::PropertyType::LINE_WIDTH:
                        ASSERT_EQ(values[k].as<prp::LineWidth>().data, lw.data);
                        break;
                    case dvs::internal::PropertyType::PERSISTENT:
                        ASSERT_EQ(values[k].as<PropertyType>(), prp::PERSISTENT);
                        break;
                    case dvs::internal::PropertyType::POINT_SIZE:
                        ASSERT_EQ(values[k].as<prp::PointSize>().data, ps.data);
                        break;
                    default:
                        ASSERT_TRUE(false) << "This should not be reached!";
                }
                break;
            default:
                ASSERT_TRUE(false) << "This should not be reached!";
        }
    }

    ASSERT_EQ(hdr.getNumValues(), 9UL);
    ASSERT_EQ(received_hdr.getNumValues(), 9UL);

    delete[] data_bytes;

    /*
    plot(x, y, Color('r'), LineWidth(1.3f), properties::PERSISTENT)
    TransmissionHeader hdr;
    hdr.append(HeaderAttributeType::FUNCTION, Function::PLOT2);
    hdr.append(HeaderAttributeType::DATA_STRUCTURE, DataStructure::VECTOR);
    hdr.append(HeaderAttributeType::DATA_TYPE, typeToDataTypeEnum<double>());
    hdr.append(HeaderAttributeType::NUM_BUFFERS_REQUIRED, static_cast<char>(2));
    hdr.append(HeaderAttributeType::ATTRIBUTE, )
    */
}

TEST_F(TestTransmissionHeader, TestBasic2)
{
    // const Color col(0.25f, 0.985f, 0.112f);

    // FunctionHeaderDataPair p;
    /*p.attribute_type = AttributeType::COLOR;
    Color c0 = p.getAsAttribute<Color>();*/

    /*fillBufferWithObjects(p.data, col);

    Color c0 = p.getAsAttribute<Color>();

    std::cout << col.red << ", " << col.green << ", " << col.blue << std::endl;
    std::cout << c0.red << ", " << c0.green << ", " << c0.blue << std::endl;*/
}
