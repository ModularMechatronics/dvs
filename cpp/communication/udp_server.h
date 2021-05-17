#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_

#include <arpa/inet.h>
#include <chrono>
#include <cstdint>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <numeric>
#include <pcap.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <queue>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdexcept>

#include "dvs.h"
#include "received_data.h"

class UdpServer
{
private:
    int port_num_;
    std::thread* receive_thread_;
    int file_descr_;
    socklen_t client_len;
    struct sockaddr_in claddr;
    struct sockaddr_in myaddr;
    char* receive_buffer_;
    std::mutex mtx_;

    std::unique_ptr<const ReceivedData> received_data_;
    std::queue<std::unique_ptr<const ReceivedData>> received_data_buffer_;

public:
    
    static constexpr size_t max_buffer_size = 1000000;

    bool hasReceivedData()
    {
        const std::lock_guard lg(mtx_);
        return received_data_buffer_.size() > 0;
    }

    UdpServer() = delete;
    UdpServer(const UdpServer& other) = delete;
    UdpServer(UdpServer&& other) = delete;

    UdpServer(const int port_num) : port_num_(port_num)
    {
        receive_buffer_ = new char[max_buffer_size];

        if((file_descr_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            throw std::runtime_error("Cannot create socket");
        }

        memset((char *)&myaddr, 0, sizeof(myaddr));
        myaddr.sin_family = AF_INET;
        myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        myaddr.sin_port = htons(port_num_);

        if(bind(file_descr_, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
        {
            throw std::runtime_error("Cannot bind");
        }
    }

    void start()
    {
        receive_thread_ = new std::thread(&UdpServer::receiveThreadFunction, this);
    }

    std::unique_ptr<const ReceivedData> getReceivedData()
    {
        const std::lock_guard lg(mtx_);
        if(received_data_buffer_.size() > 0)
        {
            std::unique_ptr<const ReceivedData> res(std::move(received_data_buffer_.front()));
            received_data_buffer_.pop();
            return res;
        }
        else
        {
            return std::unique_ptr<const ReceivedData>();
        }
    }

    void receiveThreadFunction()
    {
        client_len = sizeof(claddr);

        bool should_run = true;
        while(should_run)
        {
            const int num_received_bytes = recvfrom(file_descr_, receive_buffer_, max_buffer_size, 0, (struct sockaddr *)&claddr, &client_len);
            if (num_received_bytes < 0)
            {
                should_run = false;
                throw std::runtime_error("recvfrom returned error!");
            }
            else if(static_cast<size_t>(num_received_bytes) >= max_buffer_size)
            {
                should_run = false;
                throw std::runtime_error("Too many bytes received!");
            }

            const uint8_t* const uint8_ptr = reinterpret_cast<const uint8_t* const>(receive_buffer_);

            // TODO: Convert to other endianness and check also
            uint64_t rec_magic_num;
            std::memcpy(&rec_magic_num, &(uint8_ptr[1]), sizeof(uint64_t));
            
            if(rec_magic_num != dvs::internal::magic_num)
            {
                throw std::runtime_error("Invalid magic number!");
            }

            received_data_ = std::make_unique<const ReceivedData>(uint8_ptr, num_received_bytes);
            {
                const std::lock_guard lg(mtx_);
                received_data_buffer_.push(std::make_unique<const ReceivedData>(uint8_ptr, num_received_bytes));
            }
        }
    }

    ~UdpServer()
    {

    }

};

#endif
