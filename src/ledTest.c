#include "simpletools.h"

#include "./lib/refs.h"

void main()
{
    //byte x = 0;
    for (int i = 4; i < 27; i++)
    {
        lo(i);
        dir_out(i);
    }

    while(true)
    {
        hi(16);
        pause(1000);
        lo(16);
        pause(1000);
        //for (int i = 0; i < 8; i++)
        //    out(i + 4, (x >> i) & 0x01);
            
        //printf("%d\n", x);
        //x++;
        //pause(1000);
    }
}