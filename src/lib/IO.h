#ifndef __io_H
#define __io_H

#include "./refs.h"

typedef struct
{
    volatile int data;
    volatile int lastVal;
    volatile bool changed;
    volatile bool newValueReady;
    char* msg;
} io_binding_t;

io_binding_t* io_add_binding(uint16_t data, char* msg);

void io_switch(volatile io_binding_t* b);
void io_adc(volatile io_binding_t* b);

#endif