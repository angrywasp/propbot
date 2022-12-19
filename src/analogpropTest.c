#include "simpletools.h"

#include "./lib/modules/AnalogProp.h"
#include "./lib/refs.h"

static int x = 0;
void main()
{
    //wait for serial port
    pause(1000);

    analogprop_context_t* cxt = analogprop_init();

    while(true)
    {
        hi(16);
        printf("The counter says it's up to %d\n", x++);
        pause(1000);
        lo(16);
        pause(1000);
    }
}