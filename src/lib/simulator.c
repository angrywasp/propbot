#include "./simulator.h"
#include "./AD7812.h"
#include "./refs.h"
#include <pthread.h>
#include <conio.h>

pin_t* pins[32] = {0};
cog_t* cogs[8] = {0};
adc_port_t* ports[8] = {0};
unsigned short selected_port;

void set_direction(int pin, int direction)
{
    if (pins[pin] == 0)
    {
        pins[pin] = (pin_t*)malloc(sizeof(pin_t));
        pins[pin]->state = 0;
    }

    pins[pin]->direction = direction;

#ifdef PRINT_PIN_STATES
    if (direction)
        printf("%d out\n", pin);
    else
        printf("%d in\n", pin);
#endif
}

void set_output(int pin, int state)
{
    if (pins[pin] == 0)
    {
        pins[pin] = (pin_t*)malloc(sizeof(pin_t));
        pins[pin]->direction = 1;
    }

    if (pins[pin]->direction == 0)
    {
        printf("ERROR: attempt to toggle state of input pin %d\n", pin);
        exit(0);
    }

    pins[pin]->state = state;

#ifdef PRINT_PIN_STATES
    if (state)
        printf("%d hi\n", pin);
    else
        printf("%d lo\n", pin);
#endif
}

unsigned int get_state(int pin)
{
    if (pins[pin] == 0)
    {
        printf("ERROR: get_state called on unitialized pin %d\n", pin);
        exit(0);
    }

    if (pins[pin]->direction == 1)
    {
        printf("ERROR: attempt to get state of an output pin %d\n", pin);
        exit(0);
    }

    return pins[pin]->state;
}

int *cog_run(void (*function)(void *par), int stacksize)
{
    for (int i = 0; i < 8; i++)
    {
        if (cogs[i] == 0)
        {
            printf("initializing cog %d\n", i);
            cogs[i] = (cog_t*)malloc(sizeof(cog_t));
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, function, NULL);
            cogs[i]->thread_id = thread_id;
            return i;
        }
    }

    printf("ERROR: Ran out of available cogs\n");
    exit(0);
}

void* input_thread_function(void *ptr)
{
    while(true)
    {
        if (kbhit())
        {
            int key_code = getch();
            if (key_code == 49) //1
            {
                if (pins[4]->state == 0)
                    high(4);
                else
                    lo(4);
            }
            else if (key_code == 113) //q
            {
                adc_increment_adc(6);
            }
            else if (key_code == 97) //a
            {
                adc_decrement_adc(6);
            }
            else if (key_code == 119) //w
            {
                adc_increment_adc(7);
            }
            else if (key_code == 115) //s
            {
                adc_decrement_adc(7);
            }
            else
                printf("%d\n", key_code);
        }
        else 
            pause(100);
    }
}

void start_input_thread()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, input_thread_function, NULL);
}

void adc_select_port(int tx)
{
    if (tx == 0x4040)
    {
        ports[selected_port]->pos = 10;
        return;
    }

    if (tx < 0x6040 || tx > 0x6740)
        return;

    selected_port = (tx - 0x6040) >> 8;

    if (ports[selected_port] == 0)
    {
        int val = selected_port * 128;
        printf("Initializing ADC port %x with value %d\n", tx, val);
        ports[selected_port] = (adc_port_t*)malloc(sizeof(adc_port_t));
        ports[selected_port]->val = val;
    }

    ports[selected_port]->pos = 10;
}

void adc_respond()
{
    int state = (ports[selected_port]->val >> --ports[selected_port]->pos) & 1;
    pins[3]->state = state;
}

void adc_increment_adc(int port)
{
    int newValue = ports[port]->val;
    newValue++;
    if (newValue > 1023)
        newValue = 1023;

    ports[port]->val = newValue;
}

void adc_decrement_adc(int port)
{
    int newValue = ports[port]->val;
    newValue--;
    if (newValue < 0)
        newValue = 0;

    ports[port]->val = newValue;
}
