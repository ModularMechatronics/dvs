#ifndef DVS_FUNCTION_HEADER_H_
#define DVS_FUNCTION_HEADER_H_

#include <limits.h>
#include <stdint.h>

#include "dvs/enumerations.h"
#include "dvs/function_header_object.h"
#include "dvs/plot_properties.h"

#define kMaxNumHeaderObjects 30

typedef struct S_FunctionHeader
{
    FunctionHeaderObject values[kMaxNumHeaderObjects];
    size_t num_objects;

} FunctionHeader;

void initFunctionHeader(FunctionHeader* const hdr)
{
    hdr->num_objects = 0;
}

void appendFunctionHeaderObject(FunctionHeader* const hdr, const FunctionHeaderObject* const hdr_obj)
{
    if (hdr->num_objects == kMaxNumHeaderObjects)
    {
        printf("Tried to append header objects to full FunctionHeader!\n");
        exit(0);
    }

    hdr->values[hdr->num_objects] = *hdr_obj;
    hdr->num_objects = hdr->num_objects + 1;
}

#endif
