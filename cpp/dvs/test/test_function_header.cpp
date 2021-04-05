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

TEST_F(TestFunctionHeader, TestBasic)
{
    FunctionHeader hdr;
    hdr.append(AttributeType::FUNCTION, Function::PLOT2);
    hdr.append(AttributeType::DATA_STRUCTURE, DataStructure::VECTOR);
    hdr.append(AttributeType::DATA_TYPE, typeToDataTypeEnum<double>());
    hdr.append(AttributeType::NUM_BUFFERS_REQUIRED, static_cast<char>(2));

    variadicTemplateFunction(hdr, Color('r'), Alpha(0.1f), Linewidth(0.4f)); // , Property::PERSISTENT

    /*
    plot(x, y, Color('r'), Linewidth(1.3f), Attribute::PERSISTENT)
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
    const Color col(0.25f, 0.985f, 0.112f);

    FunctionHeaderDataPair p;
    /*p.attribute_type = AttributeType::COLOR;
    Color c0 = p.getAsAttribute<Color>();*/

    fillBufferWithObjects(p.data, col);

    Color c0 = p.getAsAttribute<Color>();

    std::cout << col.red << ", " << col.green << ", " << col.blue << std::endl;
    std::cout << c0.red << ", " << c0.green << ", " << c0.blue << std::endl;
}
