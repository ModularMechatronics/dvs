#ifndef UTILITIES_COMMANDLINE_COLORS_H_
#define UTILITIES_COMMANDLINE_COLORS_H_

#include <string>
#include <vector>

namespace arl
{
namespace command_line_color_string
{
const std::string reset = "\033[0m";

namespace fg
{
const std::string black = "\033[30m";
const std::string red = "\033[31m";
const std::string green = "\033[32m";
const std::string yellow = "\033[33m";
const std::string blue = "\033[34m";
const std::string magenta = "\033[35m";
const std::string cyan = "\033[36m";
const std::string white = "\033[37m";

}  // namespace fg

namespace bg
{
const std::string black = "\033[40m";
const std::string red = "\033[41m";
const std::string green = "\033[42m";
const std::string yellow = "\033[43m";
const std::string blue = "\033[44m";
const std::string magenta = "\033[45m";
const std::string cyan = "\033[46m";
const std::string white = "\033[47m";

}  // namespace bg

}  // namespace command_line_color_string

}  // namespace arl

#endif
