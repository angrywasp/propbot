#ifdef DESKTOP
    #include "./simulator.h"
#endif

#include "./AD7812.h"
#include "./refs.h"

const int ad7812_A1 = 0x6040;
const int ad7812_A2 = 0x6140;
const int ad7812_A3 = 0x6240;
const int ad7812_A4 = 0x6340;
const int ad7812_A5 = 0x6440;
const int ad7812_A6 = 0x6540;
const int ad7812_A7 = 0x6640;
const int ad7812_A8 = 0x6740;

static int adc_mosiPin;
static int adc_misoPin;
static int adc_clkPin;
static int adc_convstPin;

static inline void _ad7812_toggle_convst()
{
    lo(adc_convstPin);
    hi(adc_convstPin);
}

static inline int _ad7812_transfer(int tx)
{
    int rx = 0;

#ifdef DESKTOP
    adc_select_port(tx);
#endif

    for (int i = 15; i >= 5; i--)
    {
        out(adc_mosiPin, (tx >> i) & 1);
        hi(adc_clkPin);
        lo(adc_clkPin);
#ifdef DESKTOP
        adc_respond(tx);
#endif
        if (in(adc_misoPin) == 1)
            rx |= 1 << i;
    }
    lo(adc_mosiPin);
    return rx;
}

void ad7812_init(int din, int dout, int sclk, int convst)
{
    adc_mosiPin = din;
    adc_misoPin = dout;
    adc_clkPin = sclk;
    adc_convstPin = convst;

    dir_out(adc_mosiPin);
    dir_in(adc_misoPin);
    dir_out(adc_clkPin);
    dir_out(adc_convstPin);

    lo(adc_mosiPin);
    lo(adc_clkPin);
    hi(adc_convstPin);
}

int ad7812_read(int port)
{
    _ad7812_toggle_convst();
    _ad7812_transfer(port);
    _ad7812_toggle_convst();
    int val = _ad7812_transfer(0x4040);
    return val >> 6;
}

void ad7812_select(int pin)
{
    dir_out(pin);
    lo(pin);
}

void ad7812_deselect(int pin)
{
    hi(pin);
}