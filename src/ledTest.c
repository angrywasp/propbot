#include "simpletools.h"

#define true 1
#define false 0

#define dira DIRA
#define outa OUTA

#define dir_out(pin) set_direction(pin, 1)
#define dir_in(pin) set_direction(pin, 0)

#define out(pin, val) set_output(pin, val)
#define in(pin) get_state(pin)

#define hi(pin) high(pin)
#define lo(pin) low(pin)

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
