#include "communication/data_receiver.h"

#include "dvs/fillable_uint8_array.h"

DataReceiver::DataReceiver()
{
    receive_buffer_ = new char[dvs::internal::kUdpServerMaxBufferSize];

    // TCP
    tcp_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);

    // Set reuse address that's already in use (probably by exited dvs instance)
    int true_val = 1;
    setsockopt(tcp_sockfd_, SOL_SOCKET, SO_REUSEADDR, &true_val, sizeof(int));

    bzero(&tcp_servaddr_, sizeof(tcp_servaddr_));

    tcp_servaddr_.sin_family = AF_INET;
    tcp_servaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_servaddr_.sin_port = htons(dvs::internal::kTcpPortNum);

    if ((bind(tcp_sockfd_, (struct sockaddr*)&tcp_servaddr_, sizeof(tcp_servaddr_))) != 0)
    {
        throw std::runtime_error("Socket bind failed...");
    }

    tcp_len_ = sizeof(tcp_cli_);

    if ((listen(tcp_sockfd_, 5)) != 0)
    {
        throw std::runtime_error("Socket listen failed...");
    }
}

ReceivedData DataReceiver::receiveAndGetDataFromTcp()
{
    tcp_connfd_ = accept(tcp_sockfd_, (struct sockaddr*)&tcp_cli_, &tcp_len_);
    if (tcp_connfd_ < 0)
    {
        throw std::runtime_error("Server accept failed...");
    }

    read(tcp_connfd_, receive_buffer_, sizeof(uint64_t));
    size_t num_expected_bytes;

    std::memcpy(&num_expected_bytes, receive_buffer_, sizeof(uint64_t));

    size_t total_num_received_bytes = 0U;

    while (true)
    {
        const ssize_t num_received_bytes =
            read(tcp_connfd_, receive_buffer_ + total_num_received_bytes, num_expected_bytes);

        total_num_received_bytes += num_received_bytes;

        if (total_num_received_bytes >= num_expected_bytes)
        {
            break;
        }
    }

    const UInt8ArrayView array_view{reinterpret_cast<uint8_t*>(receive_buffer_), total_num_received_bytes};

    uint64_t received_magic_num;
    std::memcpy(&received_magic_num, array_view.data() + 1, sizeof(uint64_t));  // +1 because first byte is endianness

    if (received_magic_num != dvs::internal::kMagicNumber)
    {
        throw std::runtime_error("Invalid magic number received!");
    }

    return ReceivedData{array_view};
}

DataReceiver::~DataReceiver() {}
