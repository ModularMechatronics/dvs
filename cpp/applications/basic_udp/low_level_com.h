#ifndef LOW_LEVEL_COM_H_
#define LOW_LEVEL_COM_H_

#include <chrono>
#include <cstdint>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <pcap.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>

#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "basic_message.h"

inline int& Var_socket_file_descr()
{
    static int socket_file_descr;
    return socket_file_descr;
}

class UdpClient
{
private:
    int sock_file_descr;
    struct sockaddr_in tx_addr;
    struct sockaddr* tx_addr_ptr;

public:
    UdpClient() : sock_file_descr(-1), tx_addr_ptr(nullptr)
    {

    }

    UdpClient(const size_t port_num)
    {
        if ( (sock_file_descr = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
        {
            perror("socket failed");
            sock_file_descr = -1;
        }

        memset( &tx_addr, 0, sizeof(tx_addr));
        tx_addr.sin_family = AF_INET;
        tx_addr.sin_port = htons(port_num);
        tx_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        tx_addr_ptr = (struct sockaddr *)&tx_addr;
    }

    ~UdpClient()
    {
        close(sock_file_descr);
    }

    void sendData(const uint8_t* const data, const size_t num_bytes)
    {
        if(sock_file_descr == -1)
        {
            perror("Invalid socket!");
        }
        else if (sendto(sock_file_descr, data, num_bytes, 0, tx_addr_ptr, sizeof(tx_addr)) < 0 )
        {
            perror("sendto failed");
        }
    }

    /*void sendData(const DataMessage& data_message)
    {
        if(sock_file_descr == -1)
        {
            perror("Invalid socket!");
            return;
        }
        const uint64_t num_bytes_per_element = 4;
        const uint64_t num_bytes = num_bytes_per_element * data_message.num_rows;
        if(sendto(sock_file_descr, data_message.data, num_bytes, 0, tx_addr_ptr, sizeof(tx_addr)) < 0 )
        {
            perror("sendto failed");
        }
    }*/
};

#endif
