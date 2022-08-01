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

void spi_init(int mosi, int miso, int clk);

unsigned short spi_transfer(unsigned short tx);

void spi_select_chip(int pin);

void spi_deselect_chip(int pin);

void spi_wake_up(int pin);

#endif