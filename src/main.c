#ifdef DESKTOP
    #include "./lib/simulator.h"
#else
    #include "simpletools.h"
#endif

#include "./lib/AD7812.h"
#include "./lib/IO.h"
#include "./lib/refs.h"

volatile io_binding_t *adc_binding7;
volatile io_binding_t *adc_binding8;
volatile io_binding_t *btn_binding;
volatile io_binding_t* bindings[3] = {0};

void adc();
void io();
void led();

int main()
{
    pause(100);

    for (int i = 4; i <= 19; i++)
        dir_in(i);

    for (int i = 20; i <= 27; i++)
        dir_out(i);

    adc_binding7 = io_add_binding(ad7812_A7, "ADC7 %d\n");
    adc_binding8 = io_add_binding(ad7812_A8, "ADC8 %d\n");
    btn_binding = io_add_binding(4, "BTN %d\n");

    bindings[0] = adc_binding7;
    bindings[1] = adc_binding8;
    bindings[2] = btn_binding;

    pause(100);
    cog_run(adc, 128);

    pause(100);
    cog_run(io, 128);

    pause(100);
    cog_run(led, 128);

    pause(1000);

    while(true)
    {
        for (int i = 0; i < 3; i++)
        {
            if (!bindings[i]->newValueReady)
                continue;

            if (!bindings[i]->changed)
                continue;

            printf(bindings[i]->msg, bindings[i]->lastVal);
            bindings[i]->newValueReady = false;
        }
    }
}

void adc()
{
    ad7812_init(2, 3, 1, 0);
    pause(100);

    while(true)
    {
        io_adc(adc_binding7);
        io_adc(adc_binding8);
    }
}

void io()
{
    while (true)
    {
        io_switch(btn_binding);
    }
}

void led()
{
    while (true)
    {
        if (!adc_binding7->newValueReady)
            continue;

        if (!adc_binding7->changed)
            continue;

        int v = adc_binding7->lastVal;
        ++v;
        v /= 32;

        for (int i = 20; i <= 27; i++)
            lo(i);

        for (int i = 0; i < v; i++)
            hi(20 + i);
    }
}
