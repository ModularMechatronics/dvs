#include <algorithm>

#include "dvs/dvs.h"
#include "test_utils.h"

using namespace dvs;

void testNewElements()
{
    setCurrentElement("view_01", ElementType::PLOT, "a tab", ElementParent::TAB);
    // setCurrentElement("view_00", ElementType::PLOT, "a tab", ElementParent::WINDOW);
}
