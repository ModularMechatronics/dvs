#ifndef MAIN_APPLICATION_COMMUNICATION_UDP_SERVER_H_
#define MAIN_APPLICATION_COMMUNICATION_UDP_SERVER_H_

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>

#include "dvs/constants.h"
#include "dvs/dvs.h"

class UdpServer
{
private:
    int port_num_;
    int socket_file_descr_;
    struct sockaddr_in client_addr_;
    struct sockaddr_in my_addr_;
    char* receive_buffer_;
    socklen_t client_len_;

public:
    UdpServer() = delete;
    UdpServer(const UdpServer& other) = delete;
    UdpServer(UdpServer&& other) = delete;

    UdpServer(const int port_num);

    void receiveAndGetData();

    size_t numObjectsInReceiveBuffer() const;
    void sendData(char data[256], const int num_bytes_to_send);
    void sendAck();

    ~UdpServer();
};

#endif  // MAIN_APPLICATION_COMMUNICATION_UDP_SERVER_H_
