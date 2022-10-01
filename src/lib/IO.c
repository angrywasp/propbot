#include "./drivers/AD7812.h"
#include "./IO.h"
#include "./refs.h"

#define JOYSTICK_DEADZONE 4

io_switch_binding_t* io_add_switch_binding(int pin, char* msg)
{
    dir_in(pin);
    io_switch_binding_t* b = (io_switch_binding_t*)malloc(sizeof(io_switch_binding_t));
    b->value = (io_binding_value_data_t*)malloc(sizeof(io_binding_value_data_t) + strlen(msg));

    b->pin = pin;

    b->value->msg = msg;
    b->value->lastVal = -1;
    b->value->changed = false;
    b->value->newValueReady = false;

    return b;
}

io_adc_binding_t* io_add_adc_binding(ad7812_context_t* adc_context, unsigned short port, char* msg)
{
    io_adc_binding_t* b = (io_adc_binding_t*)malloc(sizeof(io_adc_binding_t));
    b->value = (io_binding_value_data_t*)malloc(sizeof(io_binding_value_data_t) + strlen(msg));

    b->port = port;
    b->adc = adc_context;

    b->value->msg = msg;
    b->value->lastVal = -1;
    b->value->changed = false;
    b->value->newValueReady = false;

    return b;
}

void io_switch(volatile io_switch_binding_t* b)
{
    if (b->value->newValueReady)
        return;

    int val = in(b->pin);

    if (val == b->value->lastVal)
    {
        b->value->changed = false;
        return;
    }

    b->value->lastVal = val;
    b->value->changed = true;
    b->value->newValueReady = true;
}

int compare(const void* a, const void* b)
{
    return *((int*)a)-*((int*)b);
}

void io_adc(volatile io_adc_binding_t* b)
{
    if (b->value->newValueReady)
        return;

    int val = 0;
    int values[5];
    for (int i = 0; i < 5; i++)
    {
        int v = ad7812_read(b->adc, b->port);
        values[i] = v >> 2;
    }

    qsort(values, 5, sizeof(int), compare);

    val = values[2];

    if (val == b->value->lastVal)
    {
        b->value->changed = false;
        return;
    }

    b->value->lastVal = val;
    b->value->changed = true;
    b->value->newValueReady = true;
}

void io_joystick(volatile io_adc_binding_t* b)
{
    if (b->value->newValueReady)
        return;

    int val = 0;
    int values[5];
    for (int i = 0; i < 5; i++)
    {
        int v = ad7812_read(b->adc, b->port);
        values[i] = v >> 2;
    }

    qsort(values, 5, sizeof(int), compare);
    val = values[2];

    if (val > 128 -JOYSTICK_DEADZONE && val < 128 + JOYSTICK_DEADZONE)
        val = 128;

    if (val == b->value->lastVal)
    {
        b->value->changed = false;
        return;
    }

    b->value->lastVal = val;
    b->value->changed = true;
    b->value->newValueReady = true;
}
