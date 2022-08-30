#ifndef __ssd1306_H
#define __ssd1306_H

#include "simplei2c.h"
#include "../refs.h"

//fonmt table constants
#define JUMPTABLE_BYTES 4

#define JUMPTABLE_LSB   1
#define JUMPTABLE_SIZE  2
#define JUMPTABLE_WIDTH 3
#define JUMPTABLE_START 4

#define WIDTH_POS 0
#define HEIGHT_POS 1
#define FIRST_CHAR_POS 2
#define CHAR_NUM_POS 3

extern const byte font_data[];

//OLED constants
#define SSD1306_WRITE_ADDRESS 0x78
#define SSD1306_CONTROL 0x00

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

typedef int SSD1306_COLOR;
typedef int SSD1306_GEOMETRY;

enum SSD1306_COLOR {
  SSD1306_COLOR_BLACK = 0,
  SSD1306_COLOR_WHITE = 1,
  SSD1306_COLOR_INVERSE = 2
};

enum SSD1306_GEOMETRY {
  SSD1306_GEOMETRY_128_64   = 0,
  SSD1306_GEOMETRY_128_32   = 1
};

typedef struct
{
    i2c *i2c_cxt;
    byte *buffer;
    SSD1306_GEOMETRY geometry;
    ushort width;
    ushort height;
    uint buffer_size;
    volatile SSD1306_COLOR color;
} ssd1306_context_t;

ssd1306_context_t* ssd1306_init(SSD1306_GEOMETRY geom, int scl, int sda);
bool ssd1306_detect(ssd1306_context_t* cxt);
void ssd1306_clear(ssd1306_context_t* cxt);
void ssd1306_reset_display(ssd1306_context_t* cxt);
void ssd1306_display(ssd1306_context_t *cxt);

void ssd1306_display_on(ssd1306_context_t *cxt);
void ssd1306_display_off(ssd1306_context_t *cxt);

ushort ssd1306_string_width(char *text, ushort text_length);
void ssd1306_txt(ssd1306_context_t* cxt, int x, int y, char *text);

void ssd1306_draw_horizontal_line(ssd1306_context_t *cxt, ushort x, ushort y, ushort length);
void ssd1306_draw_vertical_line(ssd1306_context_t *cxt, ushort x, ushort y, ushort length);
void ssd1306_draw_rect(ssd1306_context_t *cxt, ushort x, ushort y, ushort width, ushort height);
void ssd1306_fill_rect(ssd1306_context_t *cxt, ushort x, ushort y, ushort width, ushort height);

#endif