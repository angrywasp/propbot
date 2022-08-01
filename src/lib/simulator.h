#ifndef __simulator_H
#define __simulator_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct
{
    int direction; //in/out
    int state; //high/low
} pin_t;

typedef struct
{
    pthread_t thread_id;
} cog_t;

typedef struct
{
    int pos;
    int val;
} adc_port_t;

#define pause(ms) sleep(ms / 1000)

void set_direction(int pin, int direction);
void set_output(int pin, int state);
unsigned int get_state(int pin);

#define high(pin) set_output(pin, 1)
#define low(pin) set_output(pin, 0)

int *cog_run(void (*function)(void *par), int stacksize);

void *input_thread_function(void *ptr);
void start_input_thread();

//adc
void adc_select_port(int tx);
void adc_respond(int tx);
void adc_increment_adc(int port);
void adc_decrement_adc(int port);

#endif