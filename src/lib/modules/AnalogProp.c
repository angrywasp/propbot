#include "./AnalogProp.h"
#include "../refs.h"
#include "../drivers/AD7812.h"

analogprop_context_t* analogprop_init()
{
    analogprop_context_t* b = (analogprop_context_t*)malloc(sizeof(analogprop_context_t));

    //default all IO pins to low output
    for (int i = 4; i < 27; i++)
    {
        lo(i);
        dir_out(i);
    }

    b->adc = ad7812_init(2, 3, 1, 0);

    return b;
}