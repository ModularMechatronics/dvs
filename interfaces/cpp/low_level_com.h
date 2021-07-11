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

#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#define DVS_PORT_NUM 9547

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
    socklen_t client_len;

public:
    UdpClient() : sock_file_descr(-1), tx_addr_ptr(nullptr)
    {

    }

    UdpClient(const uint64_t port_num)
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

    int receiveData(char data[256])
    {
        client_len = sizeof(tx_addr);
        const int num_received_bytes = recvfrom(sock_file_descr,
                                                data,
                                                256, 0, (struct sockaddr *)&tx_addr, &client_len);
        return num_received_bytes;
    }

    void sendData(const uint8_t* const data, const uint64_t num_bytes)
    {

        /*for(size_t k = 0; k < 100; k++)
        {
            std::cout << static_cast<int>(data[k]) << std::endl;
        }*/
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
