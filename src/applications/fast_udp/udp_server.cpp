#include "udp_server.h"

#include <chrono>

#include "common.h"
#include "dvs/fillable_uint8_array.h"

constexpr size_t kUdpServerMaxBufferSizeLocal = 100000000U;

UdpServer::UdpServer(const int port_num) : port_num_(port_num)
{
    receive_buffer_ = new char[kUdpServerMaxBufferSizeLocal];

    if ((socket_file_descr_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        throw std::runtime_error("Cannot create socket");
    }

    memset((char*)&my_addr_, 0, sizeof(my_addr_));
    my_addr_.sin_family = AF_INET;
    my_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr_.sin_port = htons(port_num_);
    client_len_ = sizeof(client_addr_);

    if (bind(socket_file_descr_, (struct sockaddr*)&my_addr_, sizeof(my_addr_)) < 0)
    {
        throw std::runtime_error("Cannot bind");
    }
}

void UdpServer::sendData(char data[256], const int num_bytes_to_send)
{
    struct sockaddr* tx_addr_ptr = reinterpret_cast<struct sockaddr*>(&client_addr_);

    if (socket_file_descr_ == -1)
    {
        throw std::runtime_error("Invalid socket!");
    }
    else if (sendto(socket_file_descr_, data, num_bytes_to_send, 0, tx_addr_ptr, sizeof(struct sockaddr_in)) < 0)
    {
        throw std::runtime_error("sendto failed!");
    }
}

void UdpServer::sendAck()
{
    char data[5];
    data[0] = 'a';
    data[1] = 'c';
    data[2] = 'k';
    data[3] = '#';
    data[4] = '\0';
    sendData(data, 5);
}

void UdpServer::receiveAndGetData()
{
    size_t num_received_bytes_total = 0;
    int num_received_bytes = recvfrom(socket_file_descr_,
                                      receive_buffer_,
                                      dvs::internal::kMaxNumBytesForOneTransmission,
                                      0,
                                      (struct sockaddr*)&client_addr_,
                                      &client_len_);

    if (num_received_bytes < 0)
    {
        throw std::runtime_error("recvfrom returned error!");
    }

    num_received_bytes_total += num_received_bytes;

    uint64_t num_expected_bytes;
    std::memcpy(&num_expected_bytes, receive_buffer_ + (sizeof(uint64_t) + 1), sizeof(uint64_t));

    if (static_cast<size_t>(num_expected_bytes) >= kUdpServerMaxBufferSizeLocal)
    {
        throw std::runtime_error("Too many bytes to receive! Client wants to send " +
                                 std::to_string(num_expected_bytes) + " bytes");
    }
    // sendAck();

    if (num_expected_bytes > dvs::internal::kMaxNumBytesForOneTransmission)
    {
        using std::chrono::duration;
        using std::chrono::duration_cast;
        using std::chrono::high_resolution_clock;
        using std::chrono::milliseconds;

        auto t1 = high_resolution_clock::now();

        int i = 0;
        uint64_t prev_sequence_number = 0;

        while (num_received_bytes_total < num_expected_bytes)
        {
            num_received_bytes = recvfrom(socket_file_descr_,
                                          receive_buffer_ + num_received_bytes_total,
                                          dvs::internal::kMaxNumBytesForOneTransmission,
                                          0,
                                          (struct sockaddr*)&client_addr_,
                                          &client_len_);
            char* data_at_pos = receive_buffer_ + num_received_bytes_total;
            uint64_t sequence_number;
            std::memcpy(&sequence_number, data_at_pos, sizeof(uint64_t));

            if ((sequence_number - prev_sequence_number) != 1)
            {
                std::cout << "sn: " << sequence_number << std::endl;
            }
            prev_sequence_number = sequence_number;
            num_received_bytes_total += num_received_bytes;
#ifdef USE_ACK
            sendAck();
#endif
            i++;
        }
        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> ms_double = t2 - t1;

        DVS_LOG_INFO() << "Received " << i << " packets, took: " << ms_double.count();
    }

    const UInt8ArrayView array_view{reinterpret_cast<uint8_t*>(receive_buffer_), num_received_bytes_total};

    uint64_t received_magic_num;
    std::memcpy(&received_magic_num, array_view.data() + 1, sizeof(uint64_t));

    if (received_magic_num != dvs::internal::kMagicNumber)
    {
        throw std::runtime_error("Invalid magic number received!");
    }

    // return ReceivedData{array_view};
}

UdpServer::~UdpServer() {}
