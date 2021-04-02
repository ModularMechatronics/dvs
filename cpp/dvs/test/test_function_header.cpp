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

TEST_F(TestFunctionHeader, TestBasic)
{
    /*FunctionHeader hdr;
    hdr.append(AttributeType::FUNCTION, Function::PLOT2);
    hdr.append(AttributeType::ATTRIBUTE, Attribute::PERSISTENT);
    hdr.append(AttributeType::DATA_STRUCTURE, DataStructure::VECTOR);
    hdr.append(AttributeType::DATA_TYPE, typeToDataTypeEnum<double>());
    hdr.append(AttributeType::NUM_BUFFERS_REQUIRED, static_cast<char>(2));*/
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
