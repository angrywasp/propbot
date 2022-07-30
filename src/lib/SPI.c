#include "simpletools.h"
#include "./SPI.h"
#include "./refs.h"

const int SPI_CHIP_SELECT = 0;

static int spi_IdleCLK;
static int spi_ActiveCLK;
static int spi_PhaseCLK;

static int spi_mosiPin;
static int spi_misoPin;
static int spi_clkPin;

void spi_init(int mosi, int miso, int clk)
{
    int iMode = 1;

    spi_IdleCLK = iMode >> 1;
    spi_ActiveCLK = !spi_IdleCLK;
    spi_PhaseCLK = iMode & 0x01;

    spi_mosiPin = mosi;
    spi_misoPin = miso;
    spi_clkPin = clk;

    dir_out(mosi);
    dir_in(miso);
    dir_out(clk);

    lo(mosi);
    out(clk, spi_IdleCLK);
}

uint16_t spi_transfer(uint16_t tx)
{
    uint16_t rx = 0;

    for (int i = 15; i >= 0; i--)
    {
        if (spi_PhaseCLK == 0)
            out(spi_mosiPin, tx >> i);

        //pause(1);
        out(spi_clkPin, spi_ActiveCLK);

        if (spi_PhaseCLK == 0)
        {
            if (get_state(spi_misoPin) == 1)
                rx |= 1 << i;
        }
        else
            out(spi_mosiPin, tx >> i);

        //pause(1);
        out(spi_clkPin, spi_IdleCLK);

        if (spi_PhaseCLK == 1)
            if (in(spi_misoPin) == 1)
                rx |= 1 << i;
    }

    return rx;
}

void spi_select_chip(int pin)
{
    dir_out(pin);
    out(pin, SPI_CHIP_SELECT);
    //pause(1);
}

void spi_deselect_chip(int pin)
{
    out(pin, !SPI_CHIP_SELECT);
    //pause(1);
}

void spi_wake_up(int pin)
{
    dir_out(pin);
    out(pin, !SPI_CHIP_SELECT);
    //pause(1);
    out(pin, SPI_CHIP_SELECT);
    out(pin, !SPI_CHIP_SELECT);
}