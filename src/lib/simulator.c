#include "./simulator.h"
#include <pthread.h>

pin_t* pins[32] = {0};
cog_t* cogs[8] = {0};

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

    /*if (pins[pin]->direction == 0)
    {
        printf("ERROR: attempt to toggle state of input pin %d\n", pin);
        exit(0);
    }*/

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
