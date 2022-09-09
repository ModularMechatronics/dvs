#ifndef DVS_COMMUNICATION_H_
#define DVS_COMMUNICATION_H_

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
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

class UdpClient
{
private:
    int sock_file_descr_;
    struct sockaddr_in tx_addr_;
    struct sockaddr* tx_addr_ptr_;

public:
    UdpClient() : sock_file_descr_(-1), tx_addr_ptr_(nullptr) {}

    UdpClient(const uint64_t port_num)
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

    ~UdpClient()
    {
        close(sock_file_descr_);
    }

    template <int N>
    int receiveData(char data[N]) const
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

#endif // DVS_COMMUNICATION_H_
