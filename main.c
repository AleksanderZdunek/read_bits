#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "read_bits.h"

#define DEBUG_PRINT(expr) do{fprintf(stderr, "%s:%d: %s: %s: 0x%X\n", __FILE__, __LINE__, __func__, #expr, expr);}while(0)

int main(int argc, char* argv[])
{
    printf("Read bits from array\n");

    uint8_t bits[] = {0xAB,0xCD, 0xEF};
    struct bit_reader_state bit_reader = bit_reader_init(bits, sizeof bits);

    //test platform byte order
    uint16_t twobytes = bits[0] << 8 | bits[1];
    DEBUG_PRINT(twobytes);
    DEBUG_PRINT(((uint8_t*)(&twobytes))[0]);
    DEBUG_PRINT(((uint8_t*)(&twobytes))[1]);

    //test read_bits
    DEBUG_PRINT(read_bits_8(&bit_reader, 4));
    DEBUG_PRINT(read_bits_8(&bit_reader, 4));
    DEBUG_PRINT(read_bits_8(&bit_reader, 4));
    DEBUG_PRINT(read_bits_8(&bit_reader, 4));
    DEBUG_PRINT(read_bits_8(&bit_reader, 4));
    DEBUG_PRINT(read_bits_8(&bit_reader, 4));

    const int grouping = 3;
    bit_reader = bit_reader_init(bits, sizeof bits);
    for(int i = 0; i < sizeof(bits) * 8; ++i)
    {
        if(0 == (i % grouping)) printf(" ");
        printf("%d", read_bits_8(&bit_reader, 1));
    }
    printf("\n");
    bit_reader = bit_reader_init(bits, sizeof bits);
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));

    bit_reader = bit_reader_init(bits, sizeof bits);
    DEBUG_PRINT(read_bits_8(&bit_reader, 8));
    DEBUG_PRINT(read_bits_8(&bit_reader, 8));
    DEBUG_PRINT(read_bits_8(&bit_reader, 8));

    bit_reader = bit_reader_init(bits, sizeof bits);
    DEBUG_PRINT(read_bits_16(&bit_reader, 4));
    DEBUG_PRINT(read_bits_16(&bit_reader, 16));
    DEBUG_PRINT(read_bits_16(&bit_reader, 4));
    DEBUG_PRINT(read_bits_16(&bit_reader, 1)); //assert on reading outside bit array
    //TODO: Figure out how to catch assertions?

    return 0;
}
