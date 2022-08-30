#include "simpletools.h"

#include "./lib/drivers/AD7812.h"
#include "./lib/IO.h"
#include "./lib/refs.h"

volatile io_adc_binding_t *adc_binding7;
volatile io_adc_binding_t *adc_binding8;

volatile io_switch_binding_t *btn_binding1;
volatile io_switch_binding_t *btn_binding2;
volatile io_switch_binding_t *btn_binding3;
volatile io_switch_binding_t *btn_binding4;
volatile io_switch_binding_t *btn_binding5;
volatile io_switch_binding_t *btn_binding6;
volatile io_switch_binding_t *btn_binding7;
volatile io_switch_binding_t *btn_binding8;
volatile io_switch_binding_t *btn_binding9;
volatile io_switch_binding_t *btn_binding10;
volatile io_switch_binding_t *btn_binding11;
volatile io_switch_binding_t *btn_binding12;
volatile io_switch_binding_t *btn_binding13;
volatile io_switch_binding_t *btn_binding14;
volatile io_switch_binding_t *btn_binding15;
volatile io_switch_binding_t *btn_binding16;
volatile io_switch_binding_t *btn_binding17;
volatile io_switch_binding_t *btn_binding18;
volatile io_switch_binding_t *btn_binding19;
volatile io_switch_binding_t *btn_binding20;

volatile io_adc_binding_t* adc_bindings[2] = {0};
volatile io_switch_binding_t* switch_bindings[20] = {0};

void adc();
void io();

int main()
{
    pause(100);

    for (int i = 4; i <= 23; i++)
        dir_in(i);

    ad7812_context_t* a = ad7812_init(2, 3, 1, 0);

    adc_binding7 = io_add_adc_binding(a, ad7812_A1, "LEFT_DDI_BRT_CTL %d\n");
    adc_binding8 = io_add_adc_binding(a, ad7812_A2, "LEFT_DDI_CONT_CTL %d\n");

    btn_binding1 = io_add_switch_binding(4, "LEFT_DDI_PB_01 %d\n");
    btn_binding2 = io_add_switch_binding(5, "LEFT_DDI_PB_02 %d\n");
    btn_binding3 = io_add_switch_binding(6, "LEFT_DDI_PB_03 %d\n");
    btn_binding4 = io_add_switch_binding(7, "LEFT_DDI_PB_04 %d\n");
    btn_binding5 = io_add_switch_binding(8, "LEFT_DDI_PB_05 %d\n");
    btn_binding6 = io_add_switch_binding(9, "LEFT_DDI_PB_06 %d\n");
    btn_binding7 = io_add_switch_binding(10, "LEFT_DDI_PB_07 %d\n");
    btn_binding8 = io_add_switch_binding(11, "LEFT_DDI_PB_08 %d\n");
    btn_binding9 = io_add_switch_binding(12, "LEFT_DDI_PB_09 %d\n");
    btn_binding10 = io_add_switch_binding(13, "LEFT_DDI_PB_10 %d\n");
    btn_binding11 = io_add_switch_binding(14, "LEFT_DDI_PB_11 %d\n");
    btn_binding12 = io_add_switch_binding(15, "LEFT_DDI_PB_12 %d\n");
    btn_binding13 = io_add_switch_binding(16, "LEFT_DDI_PB_13 %d\n");
    btn_binding14 = io_add_switch_binding(17, "LEFT_DDI_PB_14 %d\n");
    btn_binding15 = io_add_switch_binding(18, "LEFT_DDI_PB_15 %d\n");
    btn_binding16 = io_add_switch_binding(19, "LEFT_DDI_PB_16 %d\n");
    btn_binding17 = io_add_switch_binding(20, "LEFT_DDI_PB_17 %d\n");
    btn_binding18 = io_add_switch_binding(21, "LEFT_DDI_PB_18 %d\n");
    btn_binding19 = io_add_switch_binding(22, "LEFT_DDI_PB_19 %d\n");
    btn_binding20 = io_add_switch_binding(23, "LEFT_DDI_PB_20 %d\n");

    adc_bindings[0] = adc_binding7;
    adc_bindings[1] = adc_binding8;

    switch_bindings[0] = btn_binding1;
    switch_bindings[1] = btn_binding2;
    switch_bindings[2] = btn_binding3;
    switch_bindings[3] = btn_binding4;
    switch_bindings[4] = btn_binding5;
    switch_bindings[5] = btn_binding6;
    switch_bindings[6] = btn_binding7;
    switch_bindings[7] = btn_binding8;
    switch_bindings[8] = btn_binding9;
    switch_bindings[9] = btn_binding10;
    switch_bindings[10] = btn_binding11;
    switch_bindings[11] = btn_binding12;
    switch_bindings[12] = btn_binding13;
    switch_bindings[13] = btn_binding14;
    switch_bindings[14] = btn_binding15;
    switch_bindings[15] = btn_binding16;
    switch_bindings[16] = btn_binding17;
    switch_bindings[17] = btn_binding18;
    switch_bindings[18] = btn_binding19;
    switch_bindings[19] = btn_binding20;

    pause(100);
    cog_run(adc, 128);

    pause(100);
    cog_run(io, 128);

    pause(1000);

    while(true)
    {
        for (int i = 0; i < 2; i++)
        {
            if (!adc_bindings[i]->value->newValueReady)
                continue;

            if (!adc_bindings[i]->value->changed)
                continue;

            printf(adc_bindings[i]->value->msg, adc_bindings[i]->value->lastVal);
            adc_bindings[i]->value->newValueReady = false;
        }

        for (int i = 0; i < 20; i++)
        {
            if (!switch_bindings[i]->value->newValueReady)
                continue;

            if (!switch_bindings[i]->value->changed)
                continue;

            printf(switch_bindings[i]->value->msg, switch_bindings[i]->value->lastVal);
            switch_bindings[i]->value->newValueReady = false;
        }
    }
}

void adc()
{
    
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
