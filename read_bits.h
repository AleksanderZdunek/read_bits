#ifndef READ_BITS_H
#define READ_BITS_H
#include <stdint.h>
#include <stddef.h>

struct bit_reader_state
{
    const uint8_t* byte_pointer;
    uint8_t bit_offset;
    const uint8_t* out_of_bounds; //marker
};

struct bit_reader_state bit_reader_init(const uint8_t* bitbuffer, size_t size);
uint8_t read_bits_8(struct bit_reader_state* state, uint8_t bits);
uint16_t read_bits_16(struct bit_reader_state* state, uint8_t bits);

#endif //READ_BITS_H
