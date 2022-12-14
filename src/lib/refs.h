#ifndef __refs_H
#define __refs_H

#include "simpletools.h"

#define bool char
#define sbyte char
#define byte unsigned char
#define ushort unsigned short
#define uint unsigned int
#define ulong unsigned long
#define true 1
#define false 0

#define dir_out(pin) set_direction(pin, 1)
#define dir_in(pin) set_direction(pin, 0)

#define out(pin, val) set_output(pin, val)
#define in(pin) get_state(pin)

#define hi(pin) high(pin)
#define lo(pin) low(pin)

#define max(a,b) (((a) > (b)) ? (a) : (b))

void ftoa(char* buffer, float val);

#endif