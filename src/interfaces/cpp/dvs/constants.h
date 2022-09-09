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
constexpr uint8_t kCommunicationHeaderObjectDataSize = SCHAR_MAX;

}
}

#endif // DVS_CONSTANTS_
