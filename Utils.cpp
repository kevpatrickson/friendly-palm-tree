#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

size_t load_file(const char *filename,  uint8_t *buffer)
{
    FILE *f = fopen(filename, "rb");
    // if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // uint8_t* buffer = malloc(size);
    // if (!buffer) {
    //     fclose(f);
    //     return NULL;
    // }

    fread(buffer, 1, size, f);
    fclose(f);

    return size;
}
