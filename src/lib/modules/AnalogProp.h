#ifndef __analogprop_H
#define __analogprop_H

#include "simplei2c.h"

#include "../drivers/AD7812.h"
#include "../refs.h"

typedef struct
{
    volatile ad7812_context_t* adc;
    volatile i2c *i2c;
} analogprop_context_t;

analogprop_context_t* analogprop_init();

#endif