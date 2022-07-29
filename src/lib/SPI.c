#include "simpletools.h"
#include "./SPI.h"
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

    set_direction(mosi, 1);
    set_direction(miso, 0);
    set_direction(clk, 1);

    set_output(mosi, 0);
    set_output(clk, spi_IdleCLK);
}

uint16_t spi_transfer(uint16_t tx)
{
    uint16_t rx = 0;

    for (int bitpos = 15; bitpos >= 0; bitpos--)
    {
        if (spi_PhaseCLK == 0)
            set_output(spi_mosiPin, tx >> bitpos);

        pause(1);
        set_output(spi_clkPin, spi_ActiveCLK);

        if (spi_PhaseCLK == 0)
        {
            if (get_state(spi_misoPin) == 1)
                rx |= 1 << bitpos;
        }
        else
            set_output(spi_mosiPin, tx >> bitpos);

        pause(1);
        set_output(spi_clkPin, spi_IdleCLK);

        if (spi_PhaseCLK == 1)
            if (get_state(spi_misoPin) == 1)
                rx |= 1 << bitpos;
    }

    return rx;
}

void spi_select_chip(int pin)
{
    set_direction(pin, 1);
    set_output(pin, SPI_CHIP_SELECT);
    pause(1);
}

void spi_deselect_chip(int pin)
{
    set_output(pin, !SPI_CHIP_SELECT);
    pause(1);
}

void spi_wake_up(int pin)
{
    set_direction(pin, 1);
    set_output(pin, !SPI_CHIP_SELECT);
    pause(1);
    set_output(pin, SPI_CHIP_SELECT);
    set_output(pin, !SPI_CHIP_SELECT);
}