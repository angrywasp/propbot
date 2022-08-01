#ifndef __ad7812_H
#define __ad7812_H

#ifdef DESKTOP
typedef struct
{
    int pos;
    int val;
} adc_port_t;

int get_adc_port(int port);
void reset_simulated_transfer(int port);
void increment_simulated_transfer(int port);
#endif

extern const int ad7812_A1;
extern const int ad7812_A2;
extern const int ad7812_A3;
extern const int ad7812_A4;
extern const int ad7812_A5;
extern const int ad7812_A6;
extern const int ad7812_A7;
extern const int ad7812_A8;

void ad7812_init(int din, int dout, int sclk, int convst);

int ad7812_read(int port);

void ad7812_select(int pin);
void ad7812_deselect(int pin);

#endif