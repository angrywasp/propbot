#ifdef DESKTOP
    #include "./simulator.h"
#else
    #include "simpletools.h"
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

#ifdef DESKTOP

unsigned short pos = 0;
unsigned short val = 512;

int get_adc_port(int port)
{
    return -1;
}

void reset_simulated_transfer(int port)
{
    pos = 15;
}
void increment_simulated_transfer(int port)
{
    //printf("%d ", pos);
    set_output(adc_misoPin, (val >> pos) & 1);
    //printf("%d ", (val >> pos) & 1);
    pos--;
    
}
/*adc_port_t* ports[8] = {0};
int get_adc_port(int port)
{
    switch(port)
    {
        case 0x6040: return 0;
        case 0x6140: return 1;
        case 0x6240: return 2;
        case 0x6340: return 3;
        case 0x6440: return 4;
        case 0x6540: return 5;
        case 0x6640: return 6;
        case 0x6740: return 7;
        default: return -1;
    }
}

void reset_simulated_transfer(int port)
{
    int p = get_adc_port(port);
    if (p == -1)
        return;

    if (ports[p] == 0)
    {
        int val = p * 128;
        printf("Initializing ADC port %x with value %d\n", port, val);
        ports[p] = (adc_port_t*)malloc(sizeof(adc_port_t));
        ports[p]->val = val;
    }

    ports[p]->pos = 15;
}

void increment_simulated_transfer(int port)
{
    int p = get_adc_port(port);
    if (p == -1)
        return;

    set_output(adc_misoPin, (ports[p]->val >> ports[p]->pos) & 1);
    ports[p]->pos--;
}*/
#endif

static inline int _ad7812_transfer(int tx)
{
    int rx = 0;

#ifdef DESKTOP
    reset_simulated_transfer(tx);
#endif

    for (int i = 15; i >= 0; i--)
    {
        out(adc_mosiPin, (tx >> i) & 1);
        hi(adc_clkPin);
        lo(adc_clkPin);
#ifdef DESKTOP
        increment_simulated_transfer(tx);
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
    return val;
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