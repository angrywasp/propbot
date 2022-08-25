#include "./HD44780.h"
#include "./refs.h"

static void _busy(volatile hd44780_context_t* cxt)
{
    bool isBusy = true;
    dir_in(cxt->d0);
    dir_in(cxt->d1);
    dir_in(cxt->d2);
    dir_in(cxt->d3);
    dir_in(cxt->d4);
    dir_in(cxt->d5);
    dir_in(cxt->d6);
    dir_in(cxt->d7);
    hi(cxt->rw);
    lo(cxt->rs);

    do
    {
        hi(cxt->e);
        isBusy = get_state(cxt->d7);
        lo(cxt->e);
    } while (isBusy);
    
    dir_out(cxt->d0);
    dir_out(cxt->d1);
    dir_out(cxt->d2);
    dir_out(cxt->d3);
    dir_out(cxt->d4);
    dir_out(cxt->d5);
    dir_out(cxt->d6);
    dir_out(cxt->d7);
}

static inline void _parallel_send(volatile hd44780_context_t* cxt, char data)
{
    out(cxt->d0, (data >> 0) & 1);
    out(cxt->d1, (data >> 1) & 1);
    out(cxt->d2, (data >> 2) & 1);
    out(cxt->d3, (data >> 3) & 1);
    out(cxt->d4, (data >> 4) & 1);
    out(cxt->d5, (data >> 5) & 1);
    out(cxt->d6, (data >> 6) & 1);
    out(cxt->d7, (data >> 7) & 1);
}

hd44780_context_t* hd44780_init(int rs, int rw, int e, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7)
{
    lo(rs);
    lo(rw);
    lo(e);
    lo(d0);
    lo(d1);
    lo(d2);
    lo(d3);
    lo(d4);
    lo(d5);
    lo(d6);
    lo(d7);

    dir_out(rs);
    dir_out(rw);
    dir_out(e);
    dir_out(d0);
    dir_out(d1);
    dir_out(d2);
    dir_out(d3);
    dir_out(d4);
    dir_out(d5);
    dir_out(d6);
    dir_out(d7);

    hd44780_context_t* cxt = (hd44780_context_t*)malloc(sizeof(hd44780_context_t));
    cxt->rs = rs;
    cxt->rw = rw;
    cxt->e = e;
    cxt->d0 = d0;
    cxt->d1 = d1;
    cxt->d2 = d2;
    cxt->d3 = d3;
    cxt->d4 = d4;
    cxt->d5 = d5;
    cxt->d6 = d6;
    cxt->d7 = d7;

    pause(15);

    //Set to DL=8 bits, N=2 lines, F=5x7 fonts
    hd44780_instruct(cxt, 0b00111000);
    hd44780_clear(cxt);
    //Display on, Cursor off, Blink off
    hd44780_instruct(cxt, 0b00001100);
    //Increment Cursor + No-Display Shift 
    hd44780_instruct(cxt, 0b00000110);

    return cxt;
}

void hd44780_clear(volatile hd44780_context_t* cxt)
{
    hd44780_instruct(cxt, 0b00000001);
}

void hd44780_instruct(volatile hd44780_context_t* cxt, char data)
{
    _busy(cxt);
    lo(cxt->rw);
    lo(cxt->rs);
    hi(cxt->e);
    _parallel_send(cxt, data);
    lo(cxt->e);
}

void hd44780_data(volatile hd44780_context_t* cxt, char data)
{
    _busy(cxt);
    lo(cxt->rw);
    hi(cxt->rs);
    hi(cxt->e);
    _parallel_send(cxt, data);
    lo(cxt->e);
}

void hd44780_move(volatile hd44780_context_t* cxt, int x, int y)
{
    int addr = (y - 1) * 64; //1 - 16
    addr += (x - 1) + 128; //1 - 2
    hd44780_instruct(cxt, addr);
}

void hd44780_str(volatile hd44780_context_t* cxt, char* string)
{
    int count = strlen(string);
    for (int i = 0; i < count; i++)
        hd44780_data(cxt, string[i]);
}

void hd44780_dec(volatile hd44780_context_t* cxt, int value)
{
    char output[10];
    sprintf(output, "%d", value);
    hd44780_str(cxt, output);
}

void hd44780_hex(volatile hd44780_context_t* cxt, int value)
{
    char output[10];
    sprintf(output, "0x%08X", value);
    hd44780_str(cxt, output);
}
