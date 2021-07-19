#ifndef DVS_INTERNAL_H_
#define DVS_INTERNAL_H_

#include <stdint.h>

#include "communication.h"

const uint64_t magic_num = 0xdeadbeefcafebabe;
const uint64_t max_bytes_for_one_msg = 1380;

int checkAck(char data[256])
{
    const int ar = data[0] == 'a' && data[1] == 'c' && data[2] == 'k' && data[3] == '#' && data[4] == '\0';
    return ar;
}

uint8_t isBigEndian()
{
    const uint32_t x = 1;
    const uint8_t* const ptr = (uint8_t*)(&x);

    if (ptr[0] == '\x01')
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint64_t minVal(const uint64_t v0, const uint64_t v1)
{
    if (v0 < v1)
    {
        return v0;
    }
    else
    {
        return v1;
    }
}

void sendThroughUdpInterface(const uint8_t* const data_blob, const uint64_t num_bytes)
{
    SocketStructure sock_struct = createSocket(9752);
    char data[256];

    if (num_bytes > max_bytes_for_one_msg)
    {
        uint64_t num_sent_bytes = 0;

        while (num_sent_bytes < num_bytes)
        {
            const uint64_t num_bytes_to_send = minVal(max_bytes_for_one_msg, num_bytes - num_sent_bytes);

            sendData(&sock_struct, &(data_blob[num_sent_bytes]), num_bytes_to_send);
            num_sent_bytes += num_bytes_to_send;

            const int num_received_bytes = receiveData(&sock_struct, data);

            const int ack_received = checkAck(data);

            if (!ack_received)
            {
                printf("Error receiving!\n");
                exit(-1);
            }
            else if (num_received_bytes != 5)
            {
                printf("Error receiving, too many bytes!\n");
                exit(-1);
            }
        }
    }
    else
    {
        sendData(&sock_struct, data_blob, num_bytes);

        const int num_received_bytes = receiveData(&sock_struct, data);

        int ack_received = checkAck(data);

        if (!ack_received)
        {
            printf("Error receiving!\n");
            exit(-1);
        }
        else if (num_received_bytes != 5)
        {
            printf("Error receiving, too many bytes!\n");
            exit(-1);
        }
    }
}

#endif
