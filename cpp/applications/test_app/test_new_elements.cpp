#include "test_utils.h"
#include "dvs.h"

#include <algorithm>

using namespace dvs;

void testNewElements()
{
    setCurrentElement("view_01", ElementType::PLOT, "a tab", ElementParent::TAB);
    // setCurrentElement("view_00", ElementType::PLOT, "a tab", ElementParent::WINDOW);
}
