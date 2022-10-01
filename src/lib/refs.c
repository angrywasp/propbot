#include "refs.h"

void ftoa(char* buffer, float val)
{
    uint x = (uint)val;
    val -= (double)x;
    uint y = (uint)(val * 1e3);

    sprintf(buffer, "%d.%d", x, y);
}