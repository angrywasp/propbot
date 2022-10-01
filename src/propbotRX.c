#include "simpletools.h"

#include "./lib/drivers/AD7812.h"
#include "./lib/drivers/NRF24L01.h"
#include "./lib/IO.h"
#include "./lib/refs.h"

static byte channel = 113;
static byte *rf_addr = (byte *)"pbot";

#define ADC_DIN 2
#define ADC_DOUT 3
#define ADC_CLK 1
#define ADC_CONVST 0

#define NRF_MOSI 10
#define NRF_MISO 11
#define NRF_CLK 12
#define NRF_CS 9
#define NRF_CE 13

#define HEADLIGHT 8                                               
#define LEFT_FWD 20
#define LEFT_REV 21
#define RIGHT_FWD 22
#define RIGHT_REV 23 
#define SHOOTER_FWD 6
#define SHOOTER_REV 7

#define PAYLOAD_SIZE 6

ad7812_context_t *adc;
nrf_context_t *nrf;

bool last_headlight = false;

static nrf_context_t *_init_receiver()
{
    nrf_context_t *rx = nrf_init(NRF_MOSI, NRF_MISO, NRF_CLK, NRF_CS, NRF_CE);

    pause(1);

    byte a = nrf_set_primary_mode(rx, 1);
    byte b = nrf_enable_rx_pipes(rx, 0x00001);
    byte c = nrf_set_shockburst(rx, 1);
    byte d = nrf_set_payload_width(rx, 0, PAYLOAD_SIZE);
    byte e = nrf_set_rf_channel(rx, channel);
    byte f = nrf_write_rx_pipe_address(rx, 0, rf_addr);
    byte g = nrf_write_tx_pipe_address(rx, rf_addr);
    printf("RX Initialized: %02x%02x%02x%02x%02x%02x%02x\n", a, b, c, d, e, f, g);

    hi(rx->ce);

    return rx;
}

int main()
{
    bool headlight = false;
    lo(HEADLIGHT);
    lo(LEFT_FWD);
    lo(LEFT_REV);
    lo(RIGHT_FWD);
    lo(RIGHT_REV);
    lo(SHOOTER_FWD);
    lo(SHOOTER_REV);

    byte *rx_dat = (byte *)malloc(PAYLOAD_SIZE);
    memset(rx_dat, 0, PAYLOAD_SIZE);
    adc = ad7812_init(ADC_DIN, ADC_DOUT, ADC_CLK, ADC_CONVST);
    nrf = _init_receiver();
    nrf_flush_rx(nrf);
    pause(1000);

    while (true)
    {
        if (!nrf_status_rx_data_ready(nrf))
        {
            printf("No incoming data\n");
            pause(1000);
            continue;
        }

        nrf_receive(nrf, rx_dat, PAYLOAD_SIZE);

        headlight = rx_dat[2];
        //if (headlight)
        //    last_headlight = !last_headlight;

        out(HEADLIGHT, headlight);
    }
}
