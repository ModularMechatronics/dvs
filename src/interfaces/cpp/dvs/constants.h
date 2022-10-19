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
constexpr uint8_t kMaxNumObjects = 10;
constexpr uint8_t kMaxNumProperties = 10;
constexpr uint8_t kMaxNumFlags = 10;
constexpr size_t kUdpServerMaxBufferSize = 100000000;
constexpr size_t kHeaderDataStartOffset = 2 * sizeof(uint64_t) + 1;

}  // namespace internal
}  // namespace dvs

#endif  // DVS_CONSTANTS_
