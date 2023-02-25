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
#include <numeric>
#include <queue>
#include <stdexcept>
#include <string>

#include "communication/received_data.h"
#include "dvs/constants.h"
#include "dvs/dvs.h"
#include "dvs/timing.h"

class DataReceiver
{
private:
    int tcp_sockfd_, tcp_connfd_;
    socklen_t tcp_len_;
    struct sockaddr_in tcp_servaddr_;
    struct sockaddr_in tcp_cli_;

public:
    DataReceiver();
    DataReceiver(const DataReceiver& other) = delete;
    DataReceiver(DataReceiver&& other) = delete;
    DataReceiver& operator=(const DataReceiver& other) = delete;
    DataReceiver& operator=(DataReceiver&& other) = delete;

    ReceivedData receiveAndGetDataFromTcp();

    ~DataReceiver();
};

#endif  // MAIN_APPLICATION_COMMUNICATION_UDP_SERVER_H_
