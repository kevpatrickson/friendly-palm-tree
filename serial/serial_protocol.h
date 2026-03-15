#ifndef _SERIAL_PROTOCOL_H_
#define _SERIAL_PROTOCOL_H_
#include <cstdint>

class SerialProtocol
{

public:
    SerialProtocol()
    {
    }

    ~SerialProtocol() {}

    float add_float(float a, float b);
    int32_t EscapeSequence(uint8_t *buffIn, uint32_t numBytes, uint8_t *buffOut,
                            uint32_t sizeBuffOut);

    int32_t UnEscapeSequence(uint8_t *buffIn, uint32_t numBytes, uint8_t *buffOut,
                            uint32_t sizeBuffOut);


private:
    const uint8_t FLAG = 0x7E;
    const uint8_t ESC = 0x7D;
    const uint8_t INV = 0x20;
    static const uint8_t NUM_CTRL_BYTES = 2;

    const uint8_t CTRL[NUM_CTRL_BYTES] = {FLAG, ESC};
};

#endif // _SERIAL_PROTOCOL_H_
