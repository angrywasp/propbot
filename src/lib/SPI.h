#ifndef __spi_H
#define __spi_H

/*
Schematics:

Propeller
+---------+
|         |                        SPI Bus
|   MOSI  +---------------------------------------------------------
|         |                |                       |               |              
|   MISO  +------------------------------------------------------------
|         |                |    |                  |  |            |  |
|    CLK  +------------------- -------------------------------------------
|         |                |    |    |             |  |  |         |  |  |
|         |              +--------------+          |  |  |         |  |  |
|         |              | MOSI MISO CLK|          |  |  |         |  |  |
|         |              |    Chip n    |        +---------+       |  |  |
|   n CS  +--------------+              |        |         |       |  |  |
|         |              +--------------+        |  Chip m |     +---------+
|   m CS  +--------------------------------------+         |     |         |
|         |                                      +---------+     | Chip l  |
|   l CS  +------------------------------------------------------+         |
|         |                                                      +---------+
+---------+
*/

#include "./refs.h"

typedef struct
{
    byte msbLsb;
    byte mode;
    byte idleClk;
    byte activeClk;
    byte phaseClk;

    int mosi;
    int miso;
    int clk;
} spi_context_t;

spi_context_t* spi_init(int mosi, int miso, int clk, byte msbLsb, byte mode);

byte spi_transfer(volatile spi_context_t* cxt, byte tx);

void spi_select_chip(int pin);
void spi_deselect_chip(int pin);
void spi_wake_up(int pin);

#endif