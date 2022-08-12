#ifdef DESKTOP
    #include "./lib/simulator.h"
#else
    #include "simpletools.h"
#endif

#include "./lib/HD44780.h"
#include "./lib/refs.h"

int main()
{
    hd44780_context_t* lcd = hd44780_init(16, 17, 18, 15, 14, 13, 12, 11, 10, 9, 8);
    hd44780_move(lcd, 3, 1);
    hd44780_str(lcd, "Hello World!");

    hd44780_clear(lcd);

    int count = 0;

    while (true)
    {
        hd44780_clear(lcd);

        hd44780_dec(lcd, count);
        hd44780_move(lcd, 1, 2);
        hd44780_hex(lcd, count);
        count += 1000;
        pause(1000);
    }
}
