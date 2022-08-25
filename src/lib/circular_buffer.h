#ifndef __circular_buffer_H
#define __circular_buffer_H

#include "./refs.h"

#define CIRCULAR_BUFFER_SIZE 24

void circular_buffer_push(ushort value);
ushort circular_buffer_pop();
void circular_buffer_lookahead(ushort *lookahead_buffer, ushort count);
bool circular_buffer_is_empty();
bool circular_buffer_is_full();

#endif