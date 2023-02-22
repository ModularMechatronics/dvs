
#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>

#include "common.h"
#include "dvs/constants.h"

class UdpClientNew
{
private:
    int sock_file_descr_;
    struct sockaddr_in tx_addr_;
    struct sockaddr* tx_addr_ptr_;

public:
    UdpClientNew() : sock_file_descr_(-1), tx_addr_ptr_(nullptr) {}

    UdpClientNew(const uint64_t port_num)
    {
        if ((sock_file_descr_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            throw std::runtime_error("socket failed!");
        }

        std::memset(&tx_addr_, 0, sizeof(tx_addr_));
        tx_addr_.sin_family = AF_INET;
        tx_addr_.sin_port = htons(port_num);
        tx_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
        tx_addr_ptr_ = (struct sockaddr*)&tx_addr_;
    }

    ~UdpClientNew()
    {
        close(sock_file_descr_);
    }

    template <int N> int receiveData(char data[N]) const
    {
        socklen_t client_len = sizeof(tx_addr_);
        const int num_received_bytes = recvfrom(sock_file_descr_, data, N, 0, (struct sockaddr*)&tx_addr_, &client_len);
        return num_received_bytes;
    }

    void sendData(const uint8_t* const data, const uint64_t num_bytes) const
    {
        if (sock_file_descr_ == -1)
        {
            throw std::runtime_error("Invalid socket!");
        }
        else if (sendto(sock_file_descr_, data, num_bytes, 0, tx_addr_ptr_, sizeof(tx_addr_)) < 0)
        {
            throw std::runtime_error("sendto failed!");
        }
    }
};

inline void sendThroughUdpInterfaceNew(const UInt8ArrayView& input_array)
{
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    const uint8_t* const data_to_send = input_array.data();
    const uint64_t total_num_bytes_to_send = input_array.size();

    constexpr int kNumReceiveBytes = 10;
    const UdpClientNew udp_client(dvs::internal::kUdpPortNum);
    char received_data[kNumReceiveBytes];

    if (total_num_bytes_to_send >= dvs::internal::kMaxNumBytesForOneTransmission)
    {
        size_t num_sent_bytes = 0;
        int i = 0;

        auto t1 = high_resolution_clock::now();

        while (num_sent_bytes < total_num_bytes_to_send)
        {
            const size_t num_bytes_to_send = std::min(dvs::internal::kMaxNumBytesForOneTransmission,
                                                      static_cast<size_t>(total_num_bytes_to_send) - num_sent_bytes);

            udp_client.sendData(data_to_send + num_sent_bytes, num_bytes_to_send);
            num_sent_bytes += num_bytes_to_send;
#ifdef USE_ACK
            const int num_received_bytes = udp_client.receiveData<kNumReceiveBytes>(received_data);

            if (!ackValid(received_data))
            {
                throw std::runtime_error("No valid ack received!");
            }
            else if (num_received_bytes != 5)
            {
                throw std::runtime_error("Ack received but number of bytes was " + std::to_string(num_received_bytes));
            }
#endif
            i++;
        }

        auto t2 = high_resolution_clock::now();

        duration<double, std::milli> ms_double = t2 - t1;

        DVS_LOG_DEBUG() << "Sent " << i << " packets, took: " << ms_double.count() << "ms";
    }
    else
    {
        udp_client.sendData(data_to_send, total_num_bytes_to_send);

        const int num_received_bytes = udp_client.receiveData<kNumReceiveBytes>(received_data);

        if (!ackValid(received_data))
        {
            throw std::runtime_error("No valid ack received!");
        }
        else if (num_received_bytes != 5)
        {
            throw std::runtime_error("Ack received but number of bytes was " + std::to_string(num_received_bytes));
        }
    }
}
