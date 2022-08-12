#ifdef DESKTOP
    #include "./lib/simulator.h"
#else
    #include "simpletools.h"
#endif

#include "./lib/NRF24L01.h"
#include "./lib/refs.h"
#include "./lib/serial_buffer.h"

void transmitter();
void receiver();

static byte channel = 113;
static byte* rf_addr = (byte*)"Hello";

int main()
{
    
    pause(1000);
    serial_buffer_push("Starting program");
    pause(100);
    
    cog_run(transmitter, 128);
    cog_run(receiver, 128);

    while (true)
    {
        if (serial_buffer_is_empty())
            continue;

        if (serial_buffer_is_full())
            printf("Serial buffer is full\n");

        char* val = serial_buffer_pop();
        if (val != 0)
            printf("%s\n", val);
    }
}

void transmitter()
{
    byte tx_val = 0;
    char output[32] = {0};


    serial_buffer_push("TX: Initializing");
    nrf_context_t* tx = nrf_init(3, 4, 2, 1, 0);

    serial_buffer_push("TX: Primary transmitter");
    nrf_set_primary_mode(tx, 0);

    serial_buffer_push("TX: Pipe0 enabled");
    nrf_enable_rx_pipes(tx, 0x00001);
    nrf_set_shockburst(tx, 0);

    serial_buffer_push("TX: Setting payload width");
    nrf_set_payload_width(tx, 0, 32);

    serial_buffer_push("TX: Setting max power");
    nrf_set_transmitter_power(tx, 0);

    serial_buffer_push("TX: Setting data rate");
    nrf_set_data_rate(tx, 1);
    nrf_set_rf_channel(tx, channel);

    //write the address
    nrf_write_rx_pipe_address(tx, 0, rf_addr);
    nrf_write_tx_pipe_address(tx, rf_addr);

    pause(1);

    while (true)
    {
        ++tx_val;

        sprintf(output, "TX: Sending - %d", tx_val);
        serial_buffer_push(output);

        //flush the tx bufferS
        nrf_flush_tx(tx);
        //clear interrupts
        nrf_read_and_clear_interrupts(tx);
        //send the value to the tx buffer
        nrf_transmit(tx, tx_val, 4);
        
        //pulse the chip to transmit
        nrf_enable(tx, true);
        pause(1);
        nrf_enable(tx, false);
        pause(1);

        if (nrf_is_data_sent(tx))
            serial_buffer_push("TX: Data sent");
        if (nrf_is_max_retries(tx))
            serial_buffer_push("TX: Max reties");
        if (nrf_is_tx_buffer_full(tx))
            serial_buffer_push("TX: TX buffer full");

        pause(1000);
    }
}

void receiver()
{
    byte rx_dat[32] = {0};

    serial_buffer_push("RX: Initializing");
    nrf_context_t* rx = nrf_init(11, 12, 10, 9, 8);

    serial_buffer_push("RX: Primary receiver");
    nrf_set_primary_mode(rx, 1);

    serial_buffer_push("RX: Pipe0 enabled");
    nrf_enable_rx_pipes(rx, 0x00001);
    nrf_set_shockburst(rx, 0);

    serial_buffer_push("RX: Setting payload width");
    nrf_set_payload_width(rx, 0, 32);
    nrf_set_rf_channel(rx, channel);

    //write the address
    nrf_write_rx_pipe_address(rx, 0, rf_addr);
    nrf_write_tx_pipe_address(rx, rf_addr);

    pause(1);

    while (true)
    {
        if (!nrf_is_payload_rx(rx))
            continue;

        nrf_receive(rx, rx_dat, 4);
        nrf_read_and_clear_interrupts(rx);
    }
}