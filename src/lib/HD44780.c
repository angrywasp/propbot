#ifdef DESKTOP
    #include "./simulator.h"
#endif

#include "./HD44780.h"
#include "./refs.h"

static int lcd_rs;
static int lcd_rw;
static int lcd_e;
static int lcd_d0;
static int lcd_d1;
static int lcd_d2;
static int lcd_d3;
static int lcd_d4;
static int lcd_d5;
static int lcd_d6;
static int lcd_d7;

static inline void _all_lo()
{
    lo(lcd_d0);
    lo(lcd_d1);
    lo(lcd_d2);
    lo(lcd_d3);
    lo(lcd_d4);
    lo(lcd_d5);
    lo(lcd_d6);
    lo(lcd_d7);
}

static inline void _all_dir_in()
{
    dir_in(lcd_d0);
    dir_in(lcd_d1);
    dir_in(lcd_d2);
    dir_in(lcd_d3);
    dir_in(lcd_d4);
    dir_in(lcd_d5);
    dir_in(lcd_d6);
    dir_in(lcd_d7);
}

static inline void _all_dir_out()
{
    dir_out(lcd_d0);
    dir_out(lcd_d1);
    dir_out(lcd_d2);
    dir_out(lcd_d3);
    dir_out(lcd_d4);
    dir_out(lcd_d5);
    dir_out(lcd_d6);
    dir_out(lcd_d7);
}

static void _busy()
{
    bool isBusy = true;
    _all_dir_in();
    hi(lcd_rw);
    lo(lcd_rs);

    do
    {
        hi(lcd_e);
        isBusy = get_state(lcd_d7);
        lo(lcd_e);
    } while (isBusy);
    _all_dir_out();
}

static inline void _parallel_send(char data)
{
    out(lcd_d0, (data >> 0) & 1);
    out(lcd_d1, (data >> 1) & 1);
    out(lcd_d2, (data >> 2) & 1);
    out(lcd_d3, (data >> 3) & 1);
    out(lcd_d4, (data >> 4) & 1);
    out(lcd_d5, (data >> 5) & 1);
    out(lcd_d6, (data >> 6) & 1);
    out(lcd_d7, (data >> 7) & 1);
}

void hd44780_init(int rs, int rw, int e, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7)
{
    lcd_rs = rs;
    lcd_rw = rw;
    lcd_e = e;
    lcd_d0 = d0;
    lcd_d1 = d1;
    lcd_d2 = d2;
    lcd_d3 = d3;
    lcd_d4 = d4;
    lcd_d5 = d5;
    lcd_d6 = d6;
    lcd_d7 = d7;

    _all_dir_out();

    dir_out(rs);
    dir_out(rw);
    dir_out(e);

    pause(15);

    _all_lo();

    lo(rs);
    lo(rw);
    lo(e);

    //Set to DL=8 bits, N=2 lines, F=5x7 fonts
    hd44780_instruct(0b00111000);
    hd44780_clear();
    //Display on, Cursor off, Blink off
    hd44780_instruct(0b00001100);
    //Increment Cursor + No-Display Shift 
    hd44780_instruct(0b00000110);
}

void hd44780_clear()
{
    hd44780_instruct(0b00000001);
}

void hd44780_instruct(char data)
{
    _busy();
    lo(lcd_rw);
    lo(lcd_rs);
    hi(lcd_e);
    _parallel_send(data);
    lo(lcd_e);
}

void hd44780_data(char data)
{
    _busy();
    lo(lcd_rw);
    hi(lcd_rs);
    hi(lcd_e);
    _parallel_send(data);
    lo(lcd_e);
}

void hd44780_move(int x, int y)
{
    int addr = (y - 1) * 64; //1 - 16
    addr += (x - 1) + 128; //1 - 2
    hd44780_instruct(addr);
}

void hd44780_str(char* string)
{
    int count = strlen(string);
    for (int i = 0; i < count; i++)
        hd44780_data(string[i]);
}

void hd44780_dec(int value)
{
    char output[10];
    sprintf(output, "%d", value);
    hd44780_str(output);
}

void hd44780_hex(int value)
{
    char output[10];
    sprintf(output, "0x%08X", value);
    hd44780_str(output);
}
