#include"./serial_buffer.h"
#include "./refs.h"

static int length;
static int read_index;
static int write_index;

static char* buffer[SERIAL_BUFFER_SIZE] = {0};

void serial_buffer_push(char* value)
{
    if (serial_buffer_is_full())
        return;

    buffer[write_index] = value;
    length++;
    write_index++;

    if (write_index == SERIAL_BUFFER_SIZE)
        write_index = 0;
}

char* serial_buffer_pop()
{
    if (serial_buffer_is_empty())
        return  0;

    char* value = buffer[read_index];
    length--;
    read_index++;

    if (read_index == SERIAL_BUFFER_SIZE)
        read_index = 0;

    return value;
}

bool serial_buffer_is_empty()
{
    return length == 0;
}

bool serial_buffer_is_full()
{
    return length == SERIAL_BUFFER_SIZE;
}