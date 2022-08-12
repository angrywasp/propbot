#include "./SPI.h"
#include "./refs.h"

const int SPI_CHIP_SELECT = 0;

static void _shift(volatile spi_context_t* cxt, byte tx, int index, byte *rx)
{
    if (cxt->phaseClk == 0)
        out(cxt->mosi, tx >> index);

    // pause(1);
    out(cxt->clk, cxt->activeClk);

    if (cxt->phaseClk == 0)
    {
        if (get_state(cxt->miso) == 1)
            (*rx) |= 1 << index;
    }
    else
        out(cxt->mosi, tx >> index);

    // pause(1);
    out(cxt->clk, cxt->idleClk);

    if (cxt->phaseClk == 1)
        if (in(cxt->miso) == 1)
            (*rx) |= 1 << index;
}

spi_context_t *spi_init(int mosi, int miso, int clk, byte msbLsb, byte mode)
{
    spi_context_t *cxt = (spi_context_t *)malloc(sizeof(spi_context_t));
    cxt->msbLsb = mode;
    cxt->mode = mode;

    cxt->idleClk = mode >> 1;
    cxt->activeClk = !cxt->idleClk;
    cxt->phaseClk = mode & 0x01;

    cxt->mosi = mosi;
    cxt->miso = miso;
    cxt->clk = clk;

    dir_out(mosi);
    dir_in(miso);
    dir_out(clk);

    lo(mosi);
    out(clk, cxt->idleClk);

    return cxt;
}

byte spi_transfer(volatile spi_context_t* cxt, byte tx)
{
    byte rx = 0;

    if (cxt->msbLsb)
    {
        for (int i = 7; i >= 0; i--)
            _shift(cxt, tx, i, &rx);
    }
    else
    {
        for (int i = 0; i < 8; i++)
            _shift(cxt, tx, i, &rx);
    }

    return rx;
}

void spi_select_chip(int pin)
{
    dir_out(pin);
    out(pin, SPI_CHIP_SELECT);
    // pause(1);
}

void spi_deselect_chip(int pin)
{
    out(pin, !SPI_CHIP_SELECT);
    // pause(1);
}

void spi_wake_up(int pin)
{
    dir_out(pin);
    out(pin, !SPI_CHIP_SELECT);
    // pause(1);
    out(pin, SPI_CHIP_SELECT);
    out(pin, !SPI_CHIP_SELECT);
}