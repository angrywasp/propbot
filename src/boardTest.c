#include "simpletools.h"

#include "./lib/refs.h"

static int x = 0;
void main()
{
    //wait for serial port
    pause(3000);

    //ad7812_context_t* a = ad7812_init(2, 3, 1, 0);

    for (int i = 4; i < 27; i++)
    {
        lo(i);
        dir_out(i);
    }

    while(true)
    {
        hi(16);
        printf("The counter says it's up to %d\n", x++);
        pause(1000);
        lo(16);
        pause(1000);
    }
}