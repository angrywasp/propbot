#ifdef DESKTOP
    #include "./lib/simulator.h"
#else
    #include "simpletools.h"
#endif

#include "./lib/refs.h"

int main()
{
    dir_out(20);
    hi(20);

    while (true)
    {
        pause(1000);
        lo(20);
        pause(1000);
        hi(20);
    }
}
