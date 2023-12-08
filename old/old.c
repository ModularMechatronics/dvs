
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printBackTrace()
{
    void* array[100];
    size_t size = backtrace(array, 100);

    backtrace_symbols_fd(array, size, STDERR_FILENO);
    std::cout << size << std::endl;
}

void holdOn()
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);
    APPEND_VAL(&hdr, CHOT_FUNCTION, F_HOLD_ON, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void holdOff()
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr);
    APPEND_VAL(&hdr, CHOT_FUNCTION, F_HOLD_OFF, uint8_t);

    sendHeader(getSendFunction(), &hdr);
}

void sendThroughUdpInterface(const uint8_t* const data_blob, const uint64_t num_bytes)
{
    SocketStructure sock_struct = createSocket(9752);
    char data[256];

    if (num_bytes > kMaxNumBytesForOneTransmission)
    {
        uint64_t num_sent_bytes = 0;

        while (num_sent_bytes < num_bytes)
        {
            const uint64_t num_bytes_to_send = minVal(kMaxNumBytesForOneTransmission, num_bytes - num_sent_bytes);

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

int checkAck(char data[256])
{
    const int ack_received = data[0] == 'a' && data[1] == 'c' && data[2] == 'k' && data[3] == '#' && data[4] == '\0';
    return ack_received;
}