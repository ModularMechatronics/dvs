#ifndef BASIC_MESSAGE_H_
#define BASIC_MESSAGE_H_

#include <stdint.h>


/*
 ----- Layer 0 -----
plot(x, y), plot(x, y, z), surf(x, y, z), rtPlot(y), imshow(img), scatter(x, y), scatter(x, y)
 ----- Layer 1 -----
sendCommand, sendScalar, sendVector, sendMatrix

 ----- Layer 2 -----
sendDataBytes
UDP, TCP, websocket, restAPI, shared memory

*/

enum class MessageType : uint8_t
{
    DATA,
    COMMAND
};

/*enum class AttributeType : uint8_t
{
    COLOR,
    LINE_WIDTH
};*/

enum class TensorType : uint8_t
{
    SCALAR,
    VECTOR,
    MATRIX,
};

enum class DataType : uint8_t
{
    FLOAT,
    DOUBLE,
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64
};

enum class CommandType
{
    PLOT,
    SURF,
    RT_PLOT
};

struct PreMessageHeader
{
    MessageType msg_type;
    uint16_t num_bytes;
    uint64_t timestamp;
    bool has_payload;
};

struct CommandMessage
{
    // plot, surf, text, imshow
    CommandType command_type;
};

struct AttributeMessage
{
    // edgecolor, linetype, facecolor
    // AttributeType attribute_type;
};

struct DataMessage
{
    TensorType tensor_type;
    DataType data_type;
    uint64_t num_rows;
    uint64_t num_cols;
    uint8_t* data;
};



#endif
