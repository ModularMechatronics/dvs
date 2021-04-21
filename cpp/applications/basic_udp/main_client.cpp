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
    Vector<uint16_t> x(20), y(20);

    for(size_t k = 0; k < 20; k++)
    {
        x(k) = k;
        y(k) = 2 * k;
    }

    setCurrentElement("view_small00");
    usleep(20000);
    view(22.4f, 0.14f);
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    /*plot(x, y, properties::Color(12, 14, 55),
               properties::Alpha(137),
               properties::Name("SimpleName"),
               properties::LineStyle("-*"),
               properties::LineWidth(22));*/
    /*setCurrentElement("view_small00");
    usleep(20000);
    setCurrentElement("view_small01");
    usleep(20000);
    setCurrentElement("view_small10");
    usleep(20000);
    setCurrentElement("view_small11");
    usleep(20000);
    setCurrentElement("view_small22");*/

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
