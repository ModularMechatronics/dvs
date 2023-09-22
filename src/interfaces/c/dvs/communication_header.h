#ifndef DVS_COMMUNICATION_HEADER_H
#define DVS_COMMUNICATION_HEADER_H

#include <limits.h>
#include <stdint.h>

#include "dvs/communication_header_object.h"
#include "dvs/enumerations.h"
#include "dvs/plot_properties.h"

#define kMaxNumHeaderObjects 30

typedef struct S_CommunicationHeader
{
    CommunicationHeaderObject values[kMaxNumHeaderObjects];
    Function function;
    size_t num_objects;

} CommunicationHeader;

void initCommunicationHeader(CommunicationHeader* const hdr, const Function function)
{
    hdr->function = function;
    hdr->num_objects = 0;
}

void appendCommunicationHeaderObject(CommunicationHeader* const hdr, const CommunicationHeaderObject* const hdr_obj)
{
    if (hdr->num_objects == kMaxNumHeaderObjects)
    {
        printf("Tried to append header objects to full CommunicationHeader!\n");
        exit(0);
    }

    hdr->values[hdr->num_objects] = *hdr_obj;
    hdr->num_objects = hdr->num_objects + 1;
}

#endif  // DVS_COMMUNICATION_HEADER_H
