#include "simpletools.h"

#include "./lib/drivers/NRF24L01.h"
#include "./lib/serial_buffer.h"
#include "./lib/refs.h"

static byte channel = 113;
static byte *rf_addr = (byte *)"Hello";

#define PAYLOAD_SIZE 4

void print_bin(unsigned a)
{
    if (a > 1)
        print_bin(a / 2);

    printf("%d", a % 2);
}

nrf_context_t *init_transmitter()
{
    nrf_context_t *tx = nrf_init(3, 4, 2, 1, 0);

    pause(1);

    byte a = nrf_set_primary_mode(tx, 0);
    byte b = nrf_enable_rx_pipes(tx, 0x00001);
    byte c = nrf_set_shockburst(tx, 1);
    byte d = nrf_set_payload_width(tx, 0, PAYLOAD_SIZE);
    byte e = nrf_set_transmitter_power(tx, 0);
    byte f = nrf_set_data_rate(tx, 1);
    byte g = nrf_set_rf_channel(tx, channel);
    byte h = nrf_write_rx_pipe_address(tx, 0, rf_addr);
    byte i = nrf_write_tx_pipe_address(tx, rf_addr);

    //printf("TX: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", a, b, c, d, e, f, g, h, i);

    pause(1);

    return tx;
}

nrf_context_t *init_receiver()
{

    nrf_context_t *rx = nrf_init(11, 12, 10, 9, 8);

    pause(1);

    byte a = nrf_set_primary_mode(rx, 1);
    byte b = nrf_enable_rx_pipes(rx, 0x00001);
    byte c = nrf_set_shockburst(rx, 1);
    byte d = nrf_set_payload_width(rx, 0, PAYLOAD_SIZE);
    byte g = nrf_set_rf_channel(rx, channel);
    byte h = nrf_write_rx_pipe_address(rx, 0, rf_addr);
    byte i = nrf_write_tx_pipe_address(rx, rf_addr);

    //printf("RX: 0x%02x 0x%02x 0x%02x 0x%02x 0x-- 0x-- 0x%02x 0x%02x 0x%02x\n", a, b, c, d, g, h, i);

    hi(rx->ce);
    pause(1);

    return rx;
}

void transmitter()
{
    pause(1000);
    unsigned int tmp = 0;
    byte* tx_dat = (byte*)malloc(PAYLOAD_SIZE);
    char* print_output = (char*)malloc(SERIAL_BUFFER_SIZE);
    nrf_context_t *tx = init_transmitter();

    while (true)
    {
        pause(5);
        ++tmp;

        tx_dat[0] = (tmp >> 24u) & 0xFF;
        tx_dat[1] = (tmp >> 16u) & 0xFF;
        tx_dat[2] = (tmp >> 8u) & 0xFF;
        tx_dat[3] = tmp & 0xFF;

        // printf("TX_DS %s\n", nrf_status_tx_data_sent(tx) ? "YES" : "NO");
        // printf("TX_FULL %s\n", nrf_fifo_status_tx_full(tx) ? "YES" : "NO");
        // printf("TX_EMPTY %s\n", nrf_fifo_status_tx_empty(tx) ? "YES" : "NO");

        
        nrf_transmit(tx, tx_dat, PAYLOAD_SIZE);

        if (nrf_status_tx_data_sent(tx))
            sprintf(print_output, "--> 0x%02x%02x%02x%02x\n", tx_dat[0], tx_dat[1], tx_dat[2], tx_dat[3]);
        else
            printf(print_output, "No data sent\n");

        serial_buffer_push(print_output);

        // printf("TX_DS %s\n", nrf_status_tx_data_sent(tx) ? "YES" : "NO");
        // printf("TX_FULL %s\n", nrf_fifo_status_tx_full(tx) ? "YES" : "NO");
        // printf("TX_EMPTY %s\n", nrf_fifo_status_tx_empty(tx) ? "YES" : "NO");
    }
}

void receiver()
{
    pause(1000);
    byte *rx_dat = (byte *)malloc(PAYLOAD_SIZE);
    char* print_output = (char*)malloc(SERIAL_BUFFER_SIZE);
    nrf_context_t *rx = init_receiver();
    nrf_flush_rx(rx);

    while (true)
    {
        // printf("RX_DR %s\n", nrf_status_rx_data_ready(rx) ? "YES" : "NO");
        // printf("RX_FULL %s\n", nrf_fifo_status_rx_full(rx) ? "YES" : "NO");
        // printf("RX_EMPTY %s\n", nrf_fifo_status_rx_empty(rx) ? "YES" : "NO");

        if (nrf_status_rx_data_ready(rx))
        {
            nrf_receive(rx, rx_dat, PAYLOAD_SIZE);
            sprintf(print_output, "<-- 0x%02x%02x%02x%02x\n", rx_dat[0], rx_dat[1], rx_dat[2], rx_dat[3]);
            serial_buffer_push(print_output);
        }

        // printf("RX_DR %s\n", nrf_status_rx_data_ready(rx) ? "YES" : "NO");
        // printf("RX_FULL %s\n", nrf_fifo_status_rx_full(rx) ? "YES" : "NO");
        // printf("RX_EMPTY %s\n", nrf_fifo_status_rx_empty(rx) ? "YES" : "NO");
    }
}

int main()
{
    cog_run(transmitter, 128);
    cog_run(receiver, 128);

    char* x = 0;

    while (true)
    {
        while (x = serial_buffer_pop())
        {
            printf(x);
        }
    }
}
