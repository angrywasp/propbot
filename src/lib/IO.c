#include "./AD7812.h"
#include "./IO.h"
#include "./refs.h"

io_binding_t* io_add_binding(unsigned short data, char* msg)
{
    io_binding_t* b = (io_binding_t*)malloc(sizeof(io_binding_t));

    b->data = data;
    b->msg = msg;
    b->lastVal = -1;
    b->changed = false;
    b->newValueReady = false;

    return b;
}

void io_switch(volatile io_binding_t* b)
{
    if (b->newValueReady)
        return;

    int val = in(b->data);

    if (val == b->lastVal)
    {
        b->changed = false;
        return;
    }

    b->lastVal = val;
    b->changed = true;
    b->newValueReady = true;
}

int compare(const void* a, const void* b)
{
    return *((int*)a)-*((int*)b);
}

void io_adc(volatile io_binding_t* b)
{
    if (b->newValueReady)
        return;

    int val = 0;
    int values[5];
    for (int i = 0; i < 5; i++)
    {
        int v = ad7812_read(b->data);
        values[i] = v >> 2;
    }

    qsort(values, 5, sizeof(int), compare);

    val = values[2];

    if (val == b->lastVal)
    {
        b->changed = false;
        return;
    }

    b->lastVal = val;
    b->changed = true;
    b->newValueReady = true;
}
