
#ifndef bits_h
#define bits_h
#include "io_com_service.h"

void get_bit(uint32 f_in,uint8 offset,uint8* ret);
void set_bit_32(uint32* f_out,uint8 offset,uint8 value);
void set_bit_16(uint16* f_out,uint8 offset,uint8 value);
void set_bit_8(uint8* f_out,uint8 offset,uint8 value);
void get_uint8(uint32 f_in,uint8 offset,uint8* ret);
void get_uint16(uint32 f_in,uint8 offset,uint16* ret);
void print_bits(uint32 f_in,uint8 size);

#endif