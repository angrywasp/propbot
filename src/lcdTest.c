#ifdef DESKTOP
    #include "./lib/simulator.h"
#else
    #include "simpletools.h"
#endif

#include "./lib/HD44780.h"
#include "./lib/refs.h"

int main()
{
    hd44780_init(16, 17, 18, 15, 14, 13, 12, 11, 10, 9, 8);
    hd44780_move(3, 1);
    hd44780_str("Hello World!");

    hd44780_clear();

    int count = 0;

    while (true)
    {
        hd44780_clear();

        hd44780_dec(count);
        hd44780_move(1, 2);
        hd44780_hex(count);
        count += 10000;
        pause(500);
    }
}
