#include"./circular_buffer.h"
#include "./refs.h"

static int length;
static int read_index;
static int write_index;

static ushort buffer[CIRCULAR_BUFFER_SIZE] = {0};

void circular_buffer_push(ushort value)
{
    if (circular_buffer_is_full())
        return;

    buffer[write_index] = value;
    length++;
    write_index++;

    if (write_index == CIRCULAR_BUFFER_SIZE)
        write_index = 0;
}

ushort circular_buffer_pop()
{
    if (circular_buffer_is_empty())
        return  0;

    ushort value = buffer[read_index];
    length--;
    read_index++;

    if (read_index == CIRCULAR_BUFFER_SIZE)
        read_index = 0;

    return value;
}

void circular_buffer_lookahead(ushort* lookahead_buffer, ushort count)
{
    if (count > CIRCULAR_BUFFER_SIZE)
        return;

    ushort index = read_index;
    for (int i = 0; i < count; i++)
    {
        lookahead_buffer[i] = buffer[index];
        index++;

        if (index == CIRCULAR_BUFFER_SIZE)
            index = 0;
    }
}

bool circular_buffer_is_empty()
{
    return length == 0;
}

bool circular_buffer_is_full()
{
    return length == CIRCULAR_BUFFER_SIZE;
}