#ifndef __serial_ring_buffer_H
#define __serial_ring_buffer_H

#include "./refs.h"

#define SERIAL_BUFFER_SIZE 32

void serial_buffer_push(char* value);
char* serial_buffer_pop();
bool serial_buffer_is_empty();
bool serial_buffer_is_full();


#endif