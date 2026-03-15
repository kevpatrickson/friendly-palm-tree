#include "serial_protocol.h"

float SerialProtocol::add_float(float a, float b)
{
    return a + b;
}

// 0x7E is encoded as 0x7E, 0x5E.  0x7D is encoded as 0x7D, 0x5D.
int32_t SerialProtocol::EscapeSequence(uint8_t *buffIn, uint32_t numBytes, uint8_t *buffOut,
                                       uint32_t sizeBuffOut)
{
    int32_t outIdx = 0;
    for (uint32_t k = 0; k < numBytes; k++)
    {
        if (outIdx < sizeBuffOut - 1)
        {
            bool escaped = false;
            uint8_t byte = buffIn[k];
            for (uint32_t m = 0; m < NUM_CTRL_BYTES; m++)
            {
                if (byte == CTRL[m])
                {
                    buffOut[outIdx++] = ESC;
                    buffOut[outIdx++] = byte ^ INV;
                    escaped = true;
                    break;
                }
            }
            if (!escaped)
            {
                buffOut[outIdx++] = byte;
            }
        }
        else
        {
            // Output array full - flag error
            return -1;
        }
    }

    return outIdx;
}

int32_t SerialProtocol::UnEscapeSequence(uint8_t *buffIn, uint32_t numBytes, uint8_t *buffOut,
                                         uint32_t sizeBuffOut)
{
    int srcIdx = 0;
    int dstIdx = 0;

    while (srcIdx < numBytes)
    {
        if (dstIdx < sizeBuffOut)
        {

            uint8_t byte = buffIn[srcIdx++];
            // for (uint32_t m = 0; m < NUM_CTRL_BYTES; m++)
            // {
            if (byte == ESC)
            {
                byte = buffIn[srcIdx++] ^ INV;
            }
            buffOut[dstIdx++] = byte;
            // }
        }
        else
        {
            // Output array full - flag error
            return -1;
        }
    }
    return dstIdx;
}