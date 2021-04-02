#include <algorithm>



#include "dvs.h"

/*
Receive from communication interface, put into data buffer
Data buffer is moved to plot window
Plot window keeps track of data

*/


using namespace dvs;

int main( void )
{
    internal::Name n("123");
    std::cout << n.data << std::endl;
    /*Color color;

    std::cout << static_cast<int>(static_cast<uint8_t>(dvs::AttributeType::NO_TYPE)) << std::endl;
    std::cout << static_cast<int>(static_cast<uint8_t>(dvs::AttributeType::COLOR)) << std::endl;
    std::cout << static_cast<int>(static_cast<uint8_t>(dvs::AttributeType::LINE_WIDTH)) << std::endl;
    std::cout << static_cast<int>(static_cast<uint8_t>(dvs::AttributeType::LINE_COLOR)) << std::endl;
    std::cout << static_cast<int>(static_cast<uint8_t>(dvs::AttributeType::FACE_COLOR)) << std::endl;*/
    // Separate functions setRtColor(<color>, <id>), setRtName(<name string>))
    for(size_t k = 0; k < 20; k++)
    {
        rtPlot(static_cast<float>(k) / 2.3f, 3, 0);
        // rtPlot(static_cast<float>(k) * 2.3f + 1.1f, 2);
    }
    /*UdpClient udp_client(9547);

    for(size_t k = 0; k < 4; k++)
    {
        uint8_t data[5] = {97, 98, 99, 100, 101};
        udp_client.sendData(data, 5);
    }*/
    /*int fd;
    if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        perror("socket failed");
        return 1;
    }

    struct sockaddr_in serveraddr;
    memset( &serveraddr, 0, sizeof(serveraddr) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( 9547 );              
    serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );  

    for ( int i = 0; i < 4; i++ ) {
        if (sendto( fd, "hello", 5, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 ) {
            perror( "sendto failed" );
            break;
        }
        printf( "message sent\n" );
    }

    close( fd );*/
}
