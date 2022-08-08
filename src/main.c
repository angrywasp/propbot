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
volatile io_binding_t *btn_binding1;
volatile io_binding_t *btn_binding2;
volatile io_binding_t *btn_binding3;
volatile io_binding_t *btn_binding4;
volatile io_binding_t *btn_binding5;
volatile io_binding_t *btn_binding6;
volatile io_binding_t *btn_binding7;
volatile io_binding_t *btn_binding8;
volatile io_binding_t *btn_binding9;
volatile io_binding_t *btn_binding10;
volatile io_binding_t *btn_binding11;
volatile io_binding_t *btn_binding12;
volatile io_binding_t *btn_binding13;
volatile io_binding_t *btn_binding14;
volatile io_binding_t *btn_binding15;
volatile io_binding_t *btn_binding16;
volatile io_binding_t *btn_binding17;
volatile io_binding_t *btn_binding18;
volatile io_binding_t *btn_binding19;
volatile io_binding_t *btn_binding20;
volatile io_binding_t* bindings[22] = {0};

void adc();
void io();
void led();

int main()
{

#ifdef DESKTOP
    start_input_thread();
#endif
    pause(100);

    for (int i = 4; i <= 23; i++)
        dir_in(i);

    adc_binding7 = io_add_binding(ad7812_A7, "LEFT_DDI_BRT_CTL %d\n");
    adc_binding8 = io_add_binding(ad7812_A8, "LEFT_DDI_CONT_CTL %d\n");

    btn_binding1 = io_add_binding(4, "LEFT_DDI_PB_01 %d\n");
    btn_binding2 = io_add_binding(5, "LEFT_DDI_PB_02 %d\n");
    btn_binding3 = io_add_binding(6, "LEFT_DDI_PB_03 %d\n");
    btn_binding4 = io_add_binding(7, "LEFT_DDI_PB_04 %d\n");
    btn_binding5 = io_add_binding(8, "LEFT_DDI_PB_05 %d\n");
    btn_binding6 = io_add_binding(9, "LEFT_DDI_PB_06 %d\n");
    btn_binding7 = io_add_binding(10, "LEFT_DDI_PB_07 %d\n");
    btn_binding8 = io_add_binding(11, "LEFT_DDI_PB_08 %d\n");
    btn_binding9 = io_add_binding(12, "LEFT_DDI_PB_09 %d\n");
    btn_binding10 = io_add_binding(13, "LEFT_DDI_PB_10 %d\n");
    btn_binding11 = io_add_binding(14, "LEFT_DDI_PB_11 %d\n");
    btn_binding12 = io_add_binding(15, "LEFT_DDI_PB_12 %d\n");
    btn_binding13 = io_add_binding(16, "LEFT_DDI_PB_13 %d\n");
    btn_binding14 = io_add_binding(17, "LEFT_DDI_PB_14 %d\n");
    btn_binding15 = io_add_binding(18, "LEFT_DDI_PB_15 %d\n");
    btn_binding16 = io_add_binding(19, "LEFT_DDI_PB_16 %d\n");
    btn_binding17 = io_add_binding(20, "LEFT_DDI_PB_17 %d\n");
    btn_binding18 = io_add_binding(21, "LEFT_DDI_PB_18 %d\n");
    btn_binding19 = io_add_binding(22, "LEFT_DDI_PB_19 %d\n");
    btn_binding20 = io_add_binding(23, "LEFT_DDI_PB_20 %d\n");

    bindings[0] = adc_binding7;
    bindings[1] = adc_binding8;

    bindings[2] = btn_binding1;
    bindings[3] = btn_binding2;
    bindings[4] = btn_binding3;
    bindings[5] = btn_binding4;
    bindings[6] = btn_binding5;
    bindings[7] = btn_binding6;
    bindings[8] = btn_binding7;
    bindings[9] = btn_binding8;
    bindings[10] = btn_binding9;
    bindings[11] = btn_binding10;
    bindings[12] = btn_binding11;
    bindings[13] = btn_binding12;
    bindings[14] = btn_binding13;
    bindings[15] = btn_binding14;
    bindings[16] = btn_binding15;
    bindings[17] = btn_binding16;
    bindings[18] = btn_binding17;
    bindings[19] = btn_binding18;
    bindings[20] = btn_binding19;
    bindings[21] = btn_binding20;

    pause(100);
    cog_run(adc, 128);

    pause(100);
    cog_run(io, 128);

    pause(1000);

    while(true)
    {
        for (int i = 0; i < 22; i++)
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
        io_switch(btn_binding1);
        io_switch(btn_binding2);
        io_switch(btn_binding3);
        io_switch(btn_binding4);
        io_switch(btn_binding5);
        io_switch(btn_binding6);
        io_switch(btn_binding7);
        io_switch(btn_binding8);
        io_switch(btn_binding9);
        io_switch(btn_binding10);
        io_switch(btn_binding11);
        io_switch(btn_binding12);
        io_switch(btn_binding13);
        io_switch(btn_binding14);
        io_switch(btn_binding15);
        io_switch(btn_binding16);
        io_switch(btn_binding17);
        io_switch(btn_binding18);
        io_switch(btn_binding19);
        io_switch(btn_binding20);
    }
}
