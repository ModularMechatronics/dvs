#ifndef DVS_GUI_API_H
#define DVS_GUI_API_H

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "dvs/gui_internal.h"

void* queryThreadFunction(void* vargp)
{
    usleep(1000U * 100U);
    internal_queryForSyncOfGuiData();

    return NULL;
}

__attribute__((weak)) void startGuiReceiveThread()
{
    initDataStructures(20U);

    internal_initTcpSocket();

    if (internal_isDvsRunning())
    {
        printf("DVS is running, starting GUI receive thread!\n");
        pthread_t query_thread;

        pthread_create(&query_thread, NULL, queryThreadFunction, NULL);
        pthread_detach(query_thread);
    }
    else
    {
        printf("DVS is not running, not starting GUI receive thread...\n");
    }

    internal_waitForSyncForAllGuiElements();
}

#endif  // DVS_GUI_API_H
