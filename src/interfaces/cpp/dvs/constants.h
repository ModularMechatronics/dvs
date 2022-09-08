#ifndef DVS_CONSTANTS_
#define DVS_CONSTANTS_

#include <stdint.h>

namespace dvs
{
namespace internal
{

constexpr uint64_t kMagicNumber = 0xdeadbeefcafebabe;
constexpr size_t kMaxNumBytesForOneTransmission = 1380;
constexpr uint64_t kUdpPortNum = 9752;

}
}

#endif // DVS_CONSTANTS_
