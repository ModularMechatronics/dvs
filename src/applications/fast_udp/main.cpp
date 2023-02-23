#include <dvs/logging.h>

#include <iostream>
#include <thread>

#include "receiver.h"
#include "sender.h"
#include "udp_server.h"

// const size_t num_elements_global = 1U * 1000U * 1000U;
const size_t num_elements_global = 7000U;
const size_t total_num_bytes_global = num_elements_global * sizeof(float);

void delayMs(const int ms)
{
    usleep(1000 * ms);
}

uint8_t* createInputData(const size_t num_elements)
{
    // 100 000 000 <- Max size, 100 million
    const size_t num_data_bytes_to_transmit = num_elements * sizeof(float);
    const size_t num_header_bytes = 1 + 2 * sizeof(uint64_t);

    uint8_t* data_uint8 = new uint8_t[num_data_bytes_to_transmit + num_header_bytes];
    size_t idx = 1U;

    // std::memcpy(dst, src, n)
    // const uint64_t kMagicNumber = 0xdeadbeefcafebabe;
    const uint64_t magic_num = dvs::internal::kMagicNumber;
    std::memcpy(data_uint8 + idx, &magic_num, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    std::memcpy(data_uint8 + idx, &num_data_bytes_to_transmit, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    float* data_float = reinterpret_cast<float*>(data_uint8 + idx);

    for (size_t k = 0; k < num_elements; k++)
    {
        data_float[k] = static_cast<float>(k) / 3.14152f;
    }

    return data_uint8;
}

void threadFunction()
{
    delayMs(500);  // Wait before starting transmission
    uint8_t* input_data = createInputData(num_elements_global);
    const UInt8ArrayView array_view(input_data, total_num_bytes_global);

    sendThroughUdpInterfaceNew(array_view);
    DVS_LOG_INFO() << "Transmission finished!";
}

int main(int argc, char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    /*float* reference_data = createInputData(num_elements_global);

    const UInt8ArrayView array_view(reinterpret_cast<uint8_t*>(reference_data), total_num_bytes_global);*/

    std::thread t0(threadFunction);

    UdpServer udp_server{dvs::internal::kUdpPortNum};

    udp_server.receiveAndGetData();
    DVS_LOG_DEBUG() << "Received finished!";

    /*while (true)
    {
        DVS_LOG_INFO() << "From main!";
        delayMs(1000);
    }*/

    // sendThroughUdpInterface(const UInt8ArrayView& input_array)

    DVS_LOG_DEBUG() << "Debug!";
    t0.join();
}
