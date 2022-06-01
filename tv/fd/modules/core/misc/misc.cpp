#include "arl/utilities/misc.h"

#include <sys/time.h>

namespace arl
{
void printStringLines(const std::vector<std::string>& string_lines)
{
    for (size_t k = 0; k < string_lines.size(); k++)
    {
        std::cout << string_lines[k];
        if (string_lines[k][string_lines[k].length() - 1] != '\n')
        {
            std::cout << std::endl;
        }
    }
}

}  // namespace arl
