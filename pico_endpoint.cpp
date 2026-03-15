#include <stdio.h>
#include "message_defs.pb.h"
#include "pico/stdlib.h"
#include <bsp/board_api.h>
#include "tusb.h"

#include <pb_encode.h>
#include <pb_decode.h>

#include "Utils.h"

void custom_cdc_task(void);

bool write_string_callback(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
    const char *str = (const char *)(*arg);
    return pb_encode_tag_for_field(stream, field) && pb_encode_string(stream, (const uint8_t *)str, strlen(str));
}

static bool decode_string_cb(pb_istream_t *stream,
                             const pb_field_t *field,
                             void **arg)
{
    char *dest = (char *)(*arg);
    size_t max_len = 63; /* leave room for '\0' */

    if (stream->bytes_left > max_len)
        return false;

    if (!pb_read(stream, (uint8_t *)dest, stream->bytes_left))
        return false;

    dest[stream->bytes_left] = '\0';
    return true;
}

bool read_string_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    // The total length of the string is known
    size_t string_len = stream->bytes_left;

    // Allocate buffer or process string piece-by-piece
    // char buffer[1024];
    char *buffer = (char *)*arg;

    // if (string_len >= sizeof(buffer)) {
    //     return false; // Or handle as an error
    // }

    // Read the string data from the stream
    if (!pb_read(stream, (uint8_t *)buffer, string_len))
    {
        return false;
    }
    buffer[string_len] = '\0'; // Null-terminate if necessary

    // Process the string, for example, print it
    printf("String received: %s\n", buffer);

    return true;
}

int main()
{

    // Initialize TinyUSB stack
    board_init();
    tusb_init();

    // TinyUSB board init callback after init
    if (board_init_after_tusb)
    {
        board_init_after_tusb();
    }

    stdio_init_all();

    printf("Running\n");

    // main run loop
    while (1)
    {
        // TinyUSB device task | must be called regurlarly
        tud_task();
        // sleep_ms(5000);
        // printf("tick\n");

        // custom tasks
        // custom_cdc_task();
    }

    int x = 0;


    /* This is the buffer where we will store our message. */
    uint8_t buffer[128];
    size_t message_length;
    bool status;

    /* Encode our message */
    {
        /* Allocate space on the stack to store the message data.
         *
         * Nanopb generates simple struct definitions for all the messages.
         * - check out the contents of simple.pb.h!
         * It is a good idea to always initialize your structures
         * so that you do not have garbage data from RAM in there.
         */
        Item message = Item_init_zero;

        // message.name.funcs.encode = write_string_callback;
        // message.name.arg = (void*)"Joes Bloggs ttyy";

        /* Create a stream that will write to our buffer. */
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        message.ID = Type_TypeA;
        message.enabled = true;
        message.parm1 = 1.23456;
        message.parm2 = 2.34567;
        message.parm3 = 3.4567;
        message.parm4 = 4.567;

        /* Now we are ready to encode the message! */
        status = pb_encode(&stream, Item_fields, &message);
        message_length = stream.bytes_written;

        /* Then just check for any errors.. */
        if (!status)
        {
            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
            return 1;
        }
    }

    /* Allocate space for the decoded message. */
    Item message1 = Item_init_zero;

    uint8_t tmpString[128];
    // message1.name.funcs.decode = read_string_callback;
    // message1.name.arg=&tmpString;

    // uint8_t testbuff[] = {0x10, 0x01, 0x1d, 0x33, 0x33, 0xb3, 0x3f};

    uint8_t testbuff[] = {0x10, 0x01, 0x1d, 0xcd, 0xcc, 0x8c, 0x3f,
                          0x25, 0x9a, 0x99, 0x99, 0x3f, 0x2d, 0x66,
                          0x66, 0xa6, 0x3f, 0x35, 0x33, 0x33, 0xb3, 0x3f};

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(testbuff, message_length);

    /* Now we are ready to decode the message. */
    status = pb_decode(&stream, Item_fields, &message1);

    /* Check for errors... */
    if (!status)
    {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        return 1;
    }

    /* Print the data contained in the message. */
    printf("parm1 was %f!\n", (float)message1.parm1);

    while (true)
    {
        printf("Hello, world! %d, %d\n", x++, message_length);
        printf("parm1 was %f!\n", (float)message1.parm1);
        sleep_ms(1000);
    }
}

void custom_cdc_task(void)
{
    // polling CDC interfaces if wanted

    // Check if CDC interface 0 (for pico sdk stdio) is connected and ready

    if (tud_cdc_n_connected(0))
    {
        // print on CDC 0 some debug message
        printf("Connected to CDC 0\n");
        sleep_ms(5000); // wait for 5 seconds
    }
}

// callback when data is received on a CDC interface
void tud_cdc_rx_cb(uint8_t itf)
{
    // allocate buffer for the data in the stack
    uint8_t buf[CFG_TUD_CDC_RX_BUFSIZE];

    printf("RX CDC %d\n", itf);

    // read the available data
    // | IMPORTANT: also do this for CDC0 because otherwise
    // | you won't be able to print anymore to CDC0
    // | next time this function is called
    uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));


if (itf == 0)
    {
        // process the received data
        buf[count] = 0; // null-terminate the string
        // now echo data back to the console on CDC 0
        printf("Received on CDC 0: %s\n", buf);
        for (int k=0;k<count;k++)
        {
            printf("%02X ", buf[k]);
        }
        printf("\n");

        // and echo back OK on CDC 1
        // tud_cdc_n_write(itf, (uint8_t const *) buf, 4);
        tud_cdc_n_write(itf, "OK", 3);
        tud_cdc_n_write_flush(itf);
    }    

    // check if the data was received on the second cdc interface
    if (itf == 1)
    {
        // process the received data
        buf[count] = 0; // null-terminate the string
        // now echo data back to the console on CDC 0
        // printf("Received on CDC 1: %s\n", buf);

        // and echo back OK on CDC 1
        // tud_cdc_n_write(itf, (uint8_t const *) buf, 4);
        // tud_cdc_n_write_flush(itf);
    }
}