#ifndef __refs_H
#define __refs_H

#include "simpletools.h"

#define bool char
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

#endif