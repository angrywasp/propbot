#include "simpletools.h"

#include "./lib/AD7812.h"
#include "./lib/IO.h"
#include "./lib/refs.h"

volatile io_adc_binding_t *vr_x;
volatile io_adc_binding_t *vr_y;
volatile io_switch_binding_t *sw;

int main()
{
    pause(1000);

    //init all IO pins to output low
    for (int i = 4; i <= 27; i++)
    {
        lo(i);
        dir_out(i);
    }

    dir_in(19);

    ad7812_context_t* adc = ad7812_init(2, 3, 1, 0);

    vr_x = io_add_adc_binding(adc, ad7812_A7, "X %d\n");
    vr_y = io_add_adc_binding(adc, ad7812_A8, "Y %d\n");

    sw = io_add_switch_binding(19, "S %d\n");

    while(true)
    {
        io_joystick(vr_x);
        io_joystick(vr_y);
        io_switch(sw);

        if (vr_x->value->newValueReady && vr_x->value->changed)
        {
            printf(vr_x->value->msg, vr_x->value->lastVal);
            vr_x->value->newValueReady = false;
        }

        if (vr_y->value->newValueReady && vr_y->value->changed)
        {
            printf(vr_y->value->msg, vr_y->value->lastVal);
            vr_y->value->newValueReady = false;
        }

        if (sw->value->newValueReady && sw->value->changed)
        {
            printf(sw->value->msg, sw->value->lastVal);
            sw->value->newValueReady = false;
        }
    }
}
