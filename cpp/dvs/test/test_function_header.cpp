#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "dvs.h"

using namespace dvs;
using namespace dvs::internal;

class TestFunctionHeader : public testing::Test
{
protected:

    void SetUp() override {}

    void TearDown() override {}
};

template <typename... Us>
void variadicTemplateFunction(FunctionHeader &hdr, const Us&... settings)
{
    hdr.extend(settings...);
}

namespace prp = dvs::properties;

TEST_F(TestFunctionHeader, TestBasic)
{
    FunctionHeader hdr;
    hdr.append(FunctionHeaderObjectType::FUNCTION, Function::PLOT2);
    hdr.append(FunctionHeaderObjectType::DATA_STRUCTURE, DataStructure::VECTOR);
    hdr.append(FunctionHeaderObjectType::DATA_TYPE, typeToDataTypeEnum<double>());
    hdr.append(FunctionHeaderObjectType::NUM_BUFFERS_REQUIRED, 2);

    variadicTemplateFunction(hdr, prp::Color(0.14f, 0.56f, 0.72f), prp::Alpha(0.1f), prp::LineWidth(0.4f), prp::PERSISTENT);

    const size_t num_bytes_required = hdr.totalNumBytesFromBuffer();

    uint8_t* data_bytes = new uint8_t[num_bytes_required];
    hdr.fillBufferWithData(data_bytes);

    const FunctionHeader received_hdr(data_bytes);

    const std::vector<FunctionHeaderObject> values = received_hdr.getValues();

    for(size_t k = 0; k < values.size(); k++)
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
            case FunctionHeaderObjectType::PROPERTY:
                switch (values[k].getAs<PropertyBase>().getPropertyType())
                {
                    case dvs::internal::PropertyType::COLOR:
                        ASSERT_EQ(values[k].getAs<prp::Color>().red, 0.14f);
                        ASSERT_EQ(values[k].getAs<prp::Color>().green, 0.56f);
                        ASSERT_EQ(values[k].getAs<prp::Color>().blue, 0.72f);
                        break;
                    case dvs::internal::PropertyType::ALPHA:
                        ASSERT_EQ(values[k].getAs<prp::Alpha>().data, 0.1f);
                        break;
                    case dvs::internal::PropertyType::LINE_WIDTH:
                        ASSERT_EQ(values[k].getAs<prp::LineWidth>().data, 0.4f);
                        break;
                    case dvs::internal::PropertyType::_PERSISTENT:
                        ASSERT_EQ(values[k].getAs<PropertyType>(), prp::PERSISTENT);
                        break;
                    default:
                        ASSERT_TRUE(false) << "This should not be reached!";
                }
                break;
            default:
                ASSERT_TRUE(false) << "This should not be reached!";

        }
    }

    ASSERT_EQ(hdr.getNumValues(), 8UL);
    ASSERT_EQ(received_hdr.getNumValues(), 8UL);

    delete[] data_bytes;

    /*
    plot(x, y, Color('r'), LineWidth(1.3f), properties::PERSISTENT)
    FunctionHeader hdr;
    hdr.append(HeaderAttributeType::FUNCTION, Function::PLOT2);
    hdr.append(HeaderAttributeType::DATA_STRUCTURE, DataStructure::VECTOR);
    hdr.append(HeaderAttributeType::DATA_TYPE, typeToDataTypeEnum<double>());
    hdr.append(HeaderAttributeType::NUM_BUFFERS_REQUIRED, static_cast<char>(2));
    hdr.append(HeaderAttributeType::ATTRIBUTE, )
    */
}

TEST_F(TestFunctionHeader, TestBasic2)
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
