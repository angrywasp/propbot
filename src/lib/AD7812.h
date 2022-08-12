#ifndef __ad7812_H
#define __ad7812_H

extern const int ad7812_A1;
extern const int ad7812_A2;
extern const int ad7812_A3;
extern const int ad7812_A4;
extern const int ad7812_A5;
extern const int ad7812_A6;
extern const int ad7812_A7;
extern const int ad7812_A8;

typedef struct
{
    volatile int mosi;
    volatile int miso;
    volatile int clk;
    volatile int convst;
} ad7812_context_t;

ad7812_context_t* ad7812_init(int din, int dout, int sclk, int convst);
int ad7812_read(volatile ad7812_context_t* cxt, int port);

#endif