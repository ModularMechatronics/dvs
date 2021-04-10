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
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <stdexcept>

#include "dvs.h"

class ReceivedData
{
private:
    dvs::internal::FunctionHeader hdr_;
    uint8_t* data_;
    uint64_t num_data_bytes_;

public:
    ReceivedData() = delete;
    ReceivedData(const ReceivedData& other) = delete;

    ReceivedData(ReceivedData&& other)
    {
        (void)other;

        data_ = other.getDataPointer();
        other.setDataPointer(nullptr);

        num_data_bytes_ = other.getNumDataBytes();
        other.setNumDataBytes(0);

        hdr_ = other.getFunctionHeader();
    }

    ReceivedData(const uint8_t* const data, const uint64_t num_received_bytes) : hdr_(&(data[2 * sizeof(uint64_t) + 1]), data[0])
    {
        const uint64_t idx = hdr_.numBytes() + 2 * sizeof(uint64_t) + 1;

        if(idx >= num_received_bytes)
        {
            throw std::runtime_error("idx can't be bigger than num_received_bytes");
        }

        num_data_bytes_ = num_received_bytes - idx;
        data_ = new uint8_t[num_data_bytes_];

    }

    uint8_t* getDataPointer() const
    {
        return data_;
    }

    void setDataPointer(uint8_t* ptr)
    {
        data_ = ptr;
    }

    void setNumDataBytes(const uint64_t ndb)
    {
        num_data_bytes_ = ndb;
    }

    uint64_t getNumDataBytes()
    {
        return num_data_bytes_;
    }

    dvs::internal::FunctionHeader getFunctionHeader()
    {
        return hdr_;
    }
};

class UdpServer
{
private:
    int port_num_;
    std::thread* receive_thread_;
    bool data_cleared_from_buffer_;
    int file_descr_;
    socklen_t client_len;
    struct sockaddr_in claddr;
    struct sockaddr_in myaddr;
    char* buf;

public:
    
    static constexpr size_t max_buffer_size = 1000000;

    UdpServer() = delete;
    UdpServer(const UdpServer& other) = delete;
    UdpServer(UdpServer&& other) = delete;

    UdpServer(const int port_num) : port_num_(port_num)
    {
        buf = new char[max_buffer_size];

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

    void receiveThreadFunction()
    {
        client_len = sizeof(claddr);

        bool should_run = true;
        while(should_run)
        {
            const int recvlen = recvfrom(file_descr_, buf, max_buffer_size, 0, (struct sockaddr *)&claddr, &client_len);
            if (recvlen < 0)
            {
                should_run = false;
                throw std::runtime_error("recvfrom returned error!");
            }
            else if(recvlen >= max_buffer_size)
            {
                should_run = false;
                throw std::runtime_error("Too many bytes received!");
            }

            /*
            uint64_t rec_magic_num;
            std::memcpy(&rec_magic_num, &(data[1]), sizeof(uint64_t));
            // TODO: Convert to other endianness and check also
            if(rec_magic_num != magic_num)
            {
                throw std::runtime_error("Invalid magic number!");
            }
            
            
            */
            
            // receiveBuffer(buf, recvlen);
        }
    }

    /*void receiveBuffer(const char* const data, const uint64_t num_received_bytes)
    {
        const uint32_t is_big_endian = data[0];

        uint64_t rec_magic_num;
        std::memcpy(&rec_magic_num, &(data[1]), sizeof(uint64_t));

        uint64_t rec_num_bytes;
        std::memcpy(&rec_num_bytes, &(data[sizeof(uint64_t) + 1]), sizeof(uint64_t));

        const uint8_t* const uint8_ptr = reinterpret_cast<const uint8_t* const>(data);

        const dvs::internal::FunctionHeader hdr(&(uint8_ptr[2 * sizeof(uint64_t) + 1]));

        std::cout << "Num values: " << hdr.getNumValues() << std::endl;
        std::cout << "Is big endian: " << is_big_endian << std::endl;
        std::cout << "magic num: " << dvs::internal::magic_num << ", rec magic num: " << rec_magic_num << std::endl;
        std::cout << "num_received_bytes: " << num_received_bytes << ", rec_num_bytes: " << rec_num_bytes << std::endl;

    }*/


    ~UdpServer()
    {

    }

};

#endif
