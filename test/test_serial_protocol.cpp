// #include "gtest/gtest.h"
// #include "gtest/gtest.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "serial_protocol.h"

// TEST(serial_math, add_float)
// {
//     SerialProtocol sm;

//     EXPECT_FLOAT_EQ(sm.add_float(1, 2.2), 3.2);
// }

TEST(SerialProtocol, EscapeSequence1)
{
    SerialProtocol sp;

    uint8_t src[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t escaped[256] = {0x00};

    uint32_t res = sp.EscapeSequence(src, sizeof(src) / sizeof(uint8_t), escaped, sizeof(escaped) / sizeof(uint8_t));
    printf("size out array: %d\n", res);

    EXPECT_FALSE(res == -1);
}

TEST(SerialProtocol, EscapeSequence2)
{
    SerialProtocol sp;

    uint8_t src[] = {0x7E, 0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t escaped[256] = {0x00};

    int32_t res = sp.EscapeSequence(src, sizeof(src) / sizeof(uint8_t), escaped, sizeof(escaped) / sizeof(uint8_t));
    EXPECT_FALSE(res == -1);

    if (res > 0)
    {
        printf("size out array: %d\n", res);
        for (int k = 0; k < res; k++)
        {
            printf("%02X,", escaped[k]);
        }
        printf("\n");
    }

    // EXPECT_FLOAT_EQ(sm.add_float(1, 2.2), 3.2);
}

TEST(SerialProtocol, EscapeSequence3)
{
    SerialProtocol sp;

    uint8_t src[] = {0x7D, 0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t escaped[256] = {0x00};
    uint8_t unEscaped[256] = {0x00};

    uint32_t res = sp.EscapeSequence(src, sizeof(src) / sizeof(uint8_t), escaped, sizeof(escaped) / sizeof(uint8_t));
    printf("size out array: %d\n", res);
    for (int k = 0; k < res; k++)
    {
        printf("%02X,", escaped[k]);
    }
    printf("\n");

    res = sp.UnEscapeSequence(escaped, res, unEscaped, sizeof(unEscaped) / sizeof(uint8_t));
    printf("size out array: %d\n", res);
    for (int k = 0; k < res; k++)
    {
        printf("%02X,", unEscaped[k]);
    }
    printf("\n");

    src[0]=0x23; // Force error

    int size = sizeof(src) / sizeof(uint8_t);
    EXPECT_THAT(
        std::vector<int>(unEscaped, unEscaped + res),
        ::testing::ElementsAreArray(src, size));

    // EXPECT_THAT(
    //     std::vector<int>(actual, actual + size),
    //     ::testing::ElementsAreArray(expected, size));
}
