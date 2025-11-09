#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define DEBUG_PRINT(expr) do{fprintf(stderr, "%s:%d: %s: %s: 0x%X\n", __FILE__, __LINE__, __func__, #expr, expr);}while(0)

struct bit_reader_state
{
    uint8_t* byte_pointer;
    uint8_t bit_offset;
};

uint8_t min(uint8_t a, uint8_t b)
{
    return a < b ? a : b;
}

uint8_t read_bits_8(struct bit_reader_state* state, uint8_t bits)
{
    assert(bits < 9);

    uint8_t bits_this_byte = min(bits, 8 - state->bit_offset);

    uint8_t byte =
        ((state->byte_pointer[0] << state->bit_offset) & 0xFF) >> (8 - bits);

    state->bit_offset += bits_this_byte;
    if(state->bit_offset > 7)
    {
        ++state->byte_pointer;
        state->bit_offset = 0;
    }

    bits -= bits_this_byte;
    if(bits)
    {
        byte |= read_bits_8(state, bits);
    }

    return byte;
}

uint16_t read_bits_16(struct bit_reader_state* state, uint8_t bits)
{
    assert(bits < 17);

    uint16_t ret = 0;
    while(bits)
    {
        uint8_t bits_this_iteration = min(bits, 8);
        bits -= bits_this_iteration;
        ret |= read_bits_8(state, bits_this_iteration) << bits;
    }
    return ret;
}

int main(int argc, char* argv[])
{
    printf("Read bits from array\n");

    uint8_t bits[] = {0xAB,0xCD, 0xEF};
    struct bit_reader_state bit_reader = {bits};

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
    bit_reader.byte_pointer = bits;
    bit_reader.bit_offset = 0;
    for(int i = 0; i < sizeof(bits) * 8; ++i)
    {
        if(0 == (i % grouping)) printf(" ");
        printf("%d", read_bits_8(&bit_reader, 1));
    }
    printf("\n");
    bit_reader.byte_pointer = bits;
    bit_reader.bit_offset = 0;
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));
    DEBUG_PRINT(read_bits_8(&bit_reader, 3));

    bit_reader.byte_pointer = bits;
    bit_reader.bit_offset = 0;
    DEBUG_PRINT(read_bits_8(&bit_reader, 8));
    DEBUG_PRINT(read_bits_8(&bit_reader, 8));
    DEBUG_PRINT(read_bits_8(&bit_reader, 8));

    bit_reader.byte_pointer = bits;
    bit_reader.bit_offset = 0;
    DEBUG_PRINT(read_bits_16(&bit_reader, 4));
    DEBUG_PRINT(read_bits_16(&bit_reader, 16));
    DEBUG_PRINT(read_bits_16(&bit_reader, 4));

    return 0;
}
