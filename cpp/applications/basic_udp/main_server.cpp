#include <algorithm>
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
#include <vector>

#include "basic_message.h"
#include "dvs.h"

#define EXPR_SIZE   1024
#define BUFLEN      4096
#define TRUE        1
#define SERVERLEN   1024

void receiveBuffer(const char* const data, const uint64_t num_received_bytes)
{
    (void)num_received_bytes;
    const uint32_t is_big_endian = data[0];

    uint64_t rec_magic_num;
    std::memcpy(&rec_magic_num, &(data[1]), sizeof(uint64_t));

    uint64_t rec_num_bytes;
    std::memcpy(&rec_num_bytes, &(data[sizeof(uint64_t) + 1]), sizeof(uint64_t));

    std::cout << "Is big endian: " << is_big_endian << std::endl << std::endl;
    std::cout << "magic num: " << dvs::internal::magic_num << ", rec magic num: " << rec_magic_num << std::endl;
    std::cout << "num_received_bytes: " << num_received_bytes << ", rec_num_bytes: " << rec_num_bytes << std::endl;

    for(size_t k = 0; k < 100; k++)
    {
        std::cout << static_cast<int>(data[k]) << std::endl;
    }

}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    struct sockaddr_in myaddr;  // address of the server
    struct sockaddr_in claddr;  // address of the client
    char* buf = new char[BUFLEN];
    int fd;
    long recvlen;
    socklen_t clientlen;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("cannot create socket");
        return 0;
    }

    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(9547);

    if(bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0){
        perror("cannot bind");
        return 0;
    }
    clientlen = sizeof(claddr);

    uint8_t* buf2 = static_cast<uint8_t*>(static_cast<void*>(buf));

    while (true) {
        recvlen = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&claddr, &clientlen);
        if (recvlen < 0) {
            perror("cannot recvfrom()");
            return 0;
        }
        receiveBuffer(buf, recvlen);
        printf("Received %ld bytes\n",recvlen);
        // buf[recvlen] = 0;
        // printf("Received message: \"%s\"\n",buf);

    }

    delete[] buf;

    return 0;
}
