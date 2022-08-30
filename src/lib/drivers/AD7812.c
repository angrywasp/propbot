#include "./AD7812.h"
#include "../refs.h"

const int ad7812_A1 = 0x6040;
const int ad7812_A2 = 0x6140;
const int ad7812_A3 = 0x6240;
const int ad7812_A4 = 0x6340;
const int ad7812_A5 = 0x6440;
const int ad7812_A6 = 0x6540;
const int ad7812_A7 = 0x6640;
const int ad7812_A8 = 0x6740;

static inline void _ad7812_toggle_convst(volatile ad7812_context_t* cxt)
{
    lo(cxt->convst);
    hi(cxt->convst);
}

static inline int _ad7812_transfer(volatile ad7812_context_t* cxt, int tx)
{
    int rx = 0;

    for (int i = 15; i >= 5; i--)
    {
        out(cxt->mosi, (tx >> i) & 1);
        hi(cxt->clk);
        lo(cxt->clk);

        if (in(cxt->miso) == 1)
            rx |= 1 << i;
    }
    lo(cxt->mosi);
    return rx;
}

ad7812_context_t* ad7812_init(int din, int dout, int sclk, int convst)
{
    lo(din);
    lo(sclk);
    hi(convst);

    dir_out(din);
    dir_in(dout);
    dir_out(sclk);
    dir_out(convst);

    ad7812_context_t* cxt = (ad7812_context_t*)malloc(sizeof(ad7812_context_t));
    cxt->mosi = din;
    cxt->miso = dout;
    cxt->clk = sclk;
    cxt->convst = convst;

    return cxt;
}

int ad7812_read(volatile ad7812_context_t* cxt, int port)
{
    _ad7812_toggle_convst(cxt);
    _ad7812_transfer(cxt, port);
    _ad7812_toggle_convst(cxt);
    int val = _ad7812_transfer(cxt, 0x4040);
    return val >> 6;
}
