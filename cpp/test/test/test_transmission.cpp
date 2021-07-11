#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "dvs.h"

using namespace dvs;
using namespace dvs::internal;
namespace prp = dvs::properties;

uint8_t buffer[2048];

class TestTransmission : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

constexpr uint64_t num_elements = 20;

template <typename... Us> FunctionHeader variadicTemplateFunction(const Us&... settings)
{
    FunctionHeader hdr;

    hdr.append(FunctionHeaderObjectType::FUNCTION, Function::PLOT2);
    hdr.append(FunctionHeaderObjectType::DATA_STRUCTURE, DataStructure::VECTOR);
    hdr.append(FunctionHeaderObjectType::DATA_TYPE, typeToDataTypeEnum<double>());
    hdr.append(FunctionHeaderObjectType::NUM_BUFFERS_REQUIRED, toUInt8(2));
    hdr.append(FunctionHeaderObjectType::NUM_ELEMENTS, toUInt32(num_elements));
    hdr.extend(settings...);

    return hdr;
}

void simpleSendFunction(const uint8_t* const data_blob, const uint64_t num_bytes)
{
    std::memcpy(buffer, data_blob, num_bytes);
}

void resetBuffer()
{
    std::memset(buffer, 0, sizeof(buffer));
}

constexpr uint64_t fcn_header_idx_start = 1 + 2 * sizeof(uint64_t);

TEST_F(TestTransmission, TestBasic)
{
    Vector<uint16_t> x(num_elements), y(num_elements);

    for (size_t k = 0; k < num_elements; k++)
    {
        x(k) = k;
        y(k) = 2 * k;
    }

    const prp::Color col(12, 14, 55);
    const prp::Alpha alpha(137);
    const prp::Name name("SimpleName");
    const prp::LineStyle ls("-*");
    const prp::LineWidth lw(27);
    const prp::PointSize ps(11);

    const FunctionHeader hdr = variadicTemplateFunction(col, alpha, name, ls, lw, ps, prp::PERSISTENT);

    resetBuffer();
    sendHeaderAndData(simpleSendFunction, hdr, x, y);

    const FunctionHeader received_header(&(buffer[fcn_header_idx_start]));

    const std::vector<FunctionHeaderObject> values = received_header.getValues();

    for (size_t k = 0; k < values.size(); k++)
    {
        switch (values[k].type)
        {
            case FunctionHeaderObjectType::FUNCTION:
                ASSERT_EQ(values[k].getAs<Function>(), Function::PLOT2);
                break;
            case FunctionHeaderObjectType::DATA_STRUCTURE:
                ASSERT_EQ(values[k].getAs<DataStructure>(), DataStructure::VECTOR);
                break;
            case FunctionHeaderObjectType::DATA_TYPE:
                ASSERT_EQ(values[k].getAs<DataType>(), DataType::DOUBLE);
                break;
            case FunctionHeaderObjectType::NUM_BUFFERS_REQUIRED:
                ASSERT_EQ(values[k].getAs<uint8_t>(), static_cast<uint8_t>(2));
                break;
            case FunctionHeaderObjectType::NUM_ELEMENTS:
                ASSERT_EQ(values[k].getAs<uint8_t>(), toUInt8(num_elements));
                break;
            case FunctionHeaderObjectType::PROPERTY:
                switch (values[k].getAs<PropertyBase>().getPropertyType())
                {
                    case dvs::internal::PropertyType::COLOR:
                        ASSERT_EQ(values[k].getAs<prp::Color>().red, col.red);
                        ASSERT_EQ(values[k].getAs<prp::Color>().green, col.green);
                        ASSERT_EQ(values[k].getAs<prp::Color>().blue, col.blue);
                        break;
                    case dvs::internal::PropertyType::ALPHA:
                        ASSERT_EQ(values[k].getAs<prp::Alpha>().data, alpha.data);
                        break;
                    case dvs::internal::PropertyType::LINE_WIDTH:
                        ASSERT_EQ(values[k].getAs<prp::LineWidth>().data, lw.data);
                        break;
                    case dvs::internal::PropertyType::PERSISTENT:
                        ASSERT_EQ(values[k].getAs<PropertyType>(), prp::PERSISTENT);
                        break;
                    case dvs::internal::PropertyType::POINT_SIZE:
                        ASSERT_EQ(values[k].getAs<prp::PointSize>().data, ps.data);
                        break;
                    case dvs::internal::PropertyType::LINE_STYLE:
                        ASSERT_EQ(std::strcmp(values[k].getAs<prp::LineStyle>().data, ls.data), 0);
                        break;
                    case dvs::internal::PropertyType::NAME:
                        ASSERT_EQ(std::strcmp(values[k].getAs<prp::Name>().data, name.data), 0);
                        break;
                    default:
                        ASSERT_TRUE(false) << "This should not be reached!";
                }
                break;
            default:
                ASSERT_TRUE(false) << "This should not be reached!";
        }
    }

    ASSERT_EQ(hdr.getNumValues(), 12UL);
    ASSERT_EQ(received_header.getNumValues(), 12UL);
    ASSERT_EQ(values.size(), 12UL);

    uint64_t data_idx = received_header.numBytes() + 1 + 2 * sizeof(uint64_t);

    Vector<uint16_t> x_rec;
    fillWithPtr(x_rec, &(buffer[data_idx]), num_elements);

    data_idx += x_rec.numBytes();

    Vector<uint16_t> y_rec;
    fillWithPtr(y_rec, &(buffer[data_idx]), num_elements);

    ASSERT_TRUE((x == x_rec).all());
    ASSERT_TRUE((y == y_rec).all());
}
