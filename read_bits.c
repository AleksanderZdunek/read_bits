#include <assert.h>
#include "read_bits.h"

struct bit_reader_state bit_reader_init(const uint8_t* bitbuffer, size_t size)
{
    return (struct bit_reader_state){
        .byte_pointer = bitbuffer,
        .bit_offset = 0,
        .out_of_bounds = bitbuffer + size,
    };
}

static uint8_t min(uint8_t a, uint8_t b)
{
    return a < b ? a : b;
}

uint8_t read_bits_8(struct bit_reader_state* state, uint8_t bits)
{
    assert(bits < 9);
    assert(state->byte_pointer < state->out_of_bounds);

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