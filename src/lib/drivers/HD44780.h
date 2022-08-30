#ifndef __hd44780_H
#define __hd44780_H
/*
INSTRUCTION SET
   ┌──────────────────────┬───┬───┬─────┬───┬───┬───┬───┬───┬───┬───┬───┬─────┬─────────────────────────────────────────────────────────────────────┐
   │  INSTRUCTION         │R/S│R/W│     │DB7│DB6│DB5│DB4│DB3│DB2│DB1│DB0│     │ Description                                                         │
   ├──────────────────────┼───┼───┼─────┼───┼───┼───┼───┼───┼───┼───┼───┼─────┼─────────────────────────────────────────────────────────────────────┤
   │ CLEAR DISPLAY        │ 0 │ 0 │     │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 1 │     │ Clears display and returns cursor to the home position (address 0). │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │ CURSOR HOME          │ 0 │ 0 │     │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 1 │ * │     │ Returns cursor to home position (address 0). Also returns display   │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │ being shifted to the original position.                             │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │ ENTRY MODE SET       │ 0 │ 0 │     │ 0 │ 0 │ 0 │ 0 │ 0 │ 1 │I/D│ S │     │ Sets cursor move direction (I/D), specifies to shift the display(S) │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │ These operations are performed during data read/write.              │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │ DISPLAY ON/OFF       │ 0 │ 0 │     │ 0 │ 0 │ 0 │ 0 │ 1 │ D │ C │ B │     │ Sets On/Off of all display (D), cursor On/Off (C) and blink of      │
   │ CONTROL              │   │   │     │   │   │   │   │   │   │   │   │     │ cursor position character (B).                                      │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │ CURSOR/DISPLAY       │ 0 │ 0 │     │ 0 │ 0 │ 0 │ 1 │S/C│R/L│ * │ * │     │ Sets cursor-move or display-shift (S/C), shift direction (R/L).     │
   │ SHIFT                │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │ FUNCTION SET         │ 0 │ 0 │     │ 0 │ 0 │ 1 │ DL│ N │ F │ * │ * │     │ Sets interface data length (DL), number of display line (N) and     │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │ character font(F).                                                  │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │ SET CGRAM ADDRESS    │ 0 │ 0 │     │ 0 │ 1 │      CGRAM ADDRESS    │     │ Sets the CGRAM address. CGRAM data is sent and received after       │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │ this setting.                                                       │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │ SET DDRAM ADDRESS    │ 0 │ 0 │     │ 1 │       DDRAM ADDRESS       │     │ Sets the DDRAM address. DDRAM data is sent and received after       │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │ this setting.                                                       │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │ READ BUSY FLAG AND   │ 0 │ 1 │     │ BF│    CGRAM/DDRAM ADDRESS    │     │ Reads Busy-flag (BF) indicating internal operation is being         │
   │ ADDRESS COUNTER      │   │   │     │   │   │   │   │   │   │   │   │     │ performed and reads CGRAM or DDRAM address counter contents.        │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │ WRITE TO CGRAM OR    │ 1 │ 0 │     │         WRITE DATA            │     │ Writes data to CGRAM or DDRAM.                                      │
   │ DDRAM                │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │ READ FROM CGRAM OR   │ 1 │ 1 │     │          READ DATA            │     │ Reads data from CGRAM or DDRAM.                                     │
   │ DDRAM                │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   │                      │   │   │     │   │   │   │   │   │   │   │   │     │                                                                     │
   └──────────────────────┴───┴───┴─────┴───┴───┴───┴───┴───┴───┴───┴───┴─────┴─────────────────────────────────────────────────────────────────────┘
   Remarks :
            * = 0 OR 1
        DDRAM = Display Data Ram
                Corresponds to cursor position
        CGRAM = Character Generator Ram

   ┌──────────┬──────────────────────────────────────────────────────────────────────┐
   │ BIT NAME │                          SETTING STATUS                              │
   ├──────────┼─────────────────────────────────┬────────────────────────────────────┤
   │  I/D     │ 0 = Decrement cursor position   │ 1 = Increment cursor position      │
   │  S       │ 0 = No display shift            │ 1 = Display shift                  │
   │  D       │ 0 = Display off                 │ 1 = Display on                     │
   │  C       │ 0 = Cursor off                  │ 1 = Cursor on                      │
   │  B       │ 0 = Cursor blink off            │ 1 = Cursor blink on                │
   │  S/C     │ 0 = Move cursor                 │ 1 = Shift display                  │
   │  R/L     │ 0 = Shift left                  │ 1 = Shift right                    │
   │  DL      │ 0 = 4-bit interface             │ 1 = 8-bit interface                │
   │  N       │ 0 = 1/8 or 1/11 Duty (1 line)   │ 1 = 1/16 Duty (2 lines)            │
   │  F       │ 0 = 5x7 dots                    │ 1 = 5x10 dots                      │
   │  BF      │ 0 = Can accept instruction      │ 1 = Internal operation in progress │
   └──────────┴─────────────────────────────────┴────────────────────────────────────┘

   DDRAM ADDRESS USAGE FOR A 1-LINE DISPLAY

    00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39   <- CHARACTER POSITION
   ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
   │00│01│02│03│04│05│06│07│08│09│0A│0B│0C│0D│0E│0F│10│11│12│13│14│15│16│17│18│19│1A│1B│1C│1D│1E│1F│20│21│22│23│24│25│26│27│  <- ROW0 DDRAM ADDRESS
   └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘

   DDRAM ADDRESS USAGE FOR A 2-LINE DISPLAY

    00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39   <- CHARACTER POSITION
   ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
   │00│01│02│03│04│05│06│07│08│09│0A│0B│0C│0D│0E│0F│10│11│12│13│14│15│16│17│18│19│1A│1B│1C│1D│1E│1F│20│21│22│23│24│25│26│27│  <- ROW0 DDRAM ADDRESS
   │40│41│42│43│44│45│46│47│48│49│4A│4B│4C│4D│4E│4F│50│51│52│53│54│55│56│57│58│59│5A│5B│5C│5D│5E│5F│60│61│62│63│64│65│66│67│  <- ROW1 DDRAM ADDRESS
   └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘

   DDRAM ADDRESS USAGE FOR A 4-LINE DISPLAY

    00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19   <- CHARACTER POSITION
   ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
   │00│01│02│03│04│05│06│07│08│09│0A│0B│0C│0D│0E│0F│10│11│12│13│  <- ROW0 DDRAM ADDRESS
   │40│41│42│43│44│45│46│47│48│49│4A│4B│4C│4D│4E│4F│50│51│52│53│  <- ROW1 DDRAM ADDRESS
   │14│15│16│17│18│19│1A│1B│1C│1D│1E│1F│20│21│22│23│24│25│26│27│  <- ROW2 DDRAM ADDRESS
   │54│55│56│57│58│59│5A│5B│5C│5D│5E│5F│60│61│62│63│64│65│66│67│  <- ROW3 DDRAM ADDRESS
   └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
*/

#include "../refs.h"

typedef struct
{
    volatile int rs;
    volatile int rw;
    volatile int e;
    volatile int d0;
    volatile int d1;
    volatile int d2;
    volatile int d3;
    volatile int d4;
    volatile int d5;
    volatile int d6;
    volatile int d7;
} hd44780_context_t;

hd44780_context_t* hd44780_init(int rs, int rw, int e, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7);
void hd44780_clear(volatile hd44780_context_t* cxt);
void hd44780_instruct(volatile hd44780_context_t* cxt, char data);
void hd44780_data(volatile hd44780_context_t* cxt, char data);
void hd44780_move(volatile hd44780_context_t* cxt, int x, int y);
void hd44780_str(volatile hd44780_context_t* cxt, char *string);
void hd44780_dec(volatile hd44780_context_t* cxt, int value);
void hd44780_hex(volatile hd44780_context_t* cxt, int value);

#endif