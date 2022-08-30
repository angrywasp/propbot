#include "simpletools.h"

#include "./lib/drivers/AD7812.h"
#include "./lib/drivers/SSD1306.h"
#include "./lib/drivers/NRF24L01.h"
#include "./lib/modules/psu.h"
#include "./lib/IO.h"
#include "./lib/refs.h"

volatile io_adc_binding_t *vr_x_l;
volatile io_adc_binding_t *vr_y_l;
volatile io_adc_binding_t *vr_x_r;
volatile io_adc_binding_t *vr_y_r;
volatile io_switch_binding_t *sw_l;
volatile io_switch_binding_t *sw_r;

volatile io_adc_binding_t *batt;

static byte channel = 113;
static byte *rf_addr = (byte *)"pbot";

#define ADC_DIN 2
#define ADC_DOUT 3
#define ADC_CLK 1
#define ADC_CONVST 0

#define OLED_SCL 6
#define OLED_SDA 5

#define NRF_MOSI 20
#define NRF_MISO 16
#define NRF_CLK 17
#define NRF_CS 21
#define NRF_CE 18

#define BATTERY_PORT ad7812_A2
#define JOY_LEFT_X_PORT ad7812_A3
#define JOY_LEFT_Y_PORT ad7812_A4
#define JOY_RIGHT_X_PORT ad7812_A5
#define JOY_RIGHT_Y_PORT ad7812_A6

#define JOY_LEFT_SW 9
#define JOY_RIGHT_SW 10

#define PAYLOAD_SIZE 6

ssd1306_context_t *oled;
ad7812_context_t *adc;
nrf_context_t *nrf;
psu_context_t * psu;

char print_out[32] = {0};

static void _partial_clear_oled(ushort x, ushort y, ushort width, ushort height)
{
    oled->color = SSD1306_COLOR_BLACK;
    ssd1306_fill_rect(oled, x, y, width, height);
    oled->color = SSD1306_COLOR_WHITE;
}

static nrf_context_t *_init_transmitter()
{
    nrf_context_t *tx = nrf_init(NRF_MOSI, NRF_MISO, NRF_CLK, NRF_CS, NRF_CE);

    //char output[21] = {0};

    byte a = nrf_set_primary_mode(tx, 0);
    byte b = nrf_enable_rx_pipes(tx, 0x00001);
    byte c = nrf_set_shockburst(tx, 1);
    byte d = nrf_set_payload_width(tx, 0, PAYLOAD_SIZE);
    byte e = nrf_set_transmitter_power(tx, 0);
    byte f = nrf_set_data_rate(tx, 0);
    byte g = nrf_set_rf_channel(tx, channel);
    byte h = nrf_write_rx_pipe_address(tx, 0, rf_addr);
    byte i = nrf_write_tx_pipe_address(tx, rf_addr);

    //sprintf(output, "%02x%02x%02x%02x%02x%02x%02x%02x%02x", a, b, c, d, e, f, g, h, i);

    //ssd1306_clear(oled);
    //ssd1306_txt(oled, 0, 0, "TX initialized");
    //ssd1306_txt(oled, 0, 16, output);
    //ssd1306_display(oled);

    return tx;
}

int main()
{
    byte *tx_dat = (byte *)malloc(PAYLOAD_SIZE);
    psu = psu_init(22000, 47000, 3.3f, 2.75f, 4.2f);
    oled = ssd1306_init(SSD1306_GEOMETRY_128_32, OLED_SCL, OLED_SDA);
    adc = ad7812_init(ADC_DIN, ADC_DOUT, ADC_CLK, ADC_CONVST);
    nrf = _init_transmitter();

    vr_x_l = io_add_adc_binding(adc, JOY_LEFT_X_PORT, 0);
    vr_y_l = io_add_adc_binding(adc, JOY_LEFT_Y_PORT, 0);
    vr_x_r = io_add_adc_binding(adc, JOY_RIGHT_X_PORT, 0);
    vr_y_r = io_add_adc_binding(adc, JOY_RIGHT_Y_PORT, 0);

    sw_l = io_add_switch_binding(JOY_LEFT_SW, 0);
    sw_r = io_add_switch_binding(JOY_RIGHT_SW, 0);

    batt = io_add_adc_binding(adc, BATTERY_PORT, 0);

    ssd1306_clear(oled);

    while (true)
    {
        io_adc(batt);
        io_joystick(vr_x_l);
        io_joystick(vr_y_l);
        io_joystick(vr_x_r);
        io_joystick(vr_y_r);
        io_switch(sw_l);
        io_switch(sw_r);

        // left joystick
        if (vr_x_l->value->newValueReady && vr_x_l->value->changed)
        {
            tx_dat[0] = vr_x_l->value->lastVal;
            vr_x_l->value->newValueReady = false;
        }

        if (vr_y_l->value->newValueReady && vr_y_l->value->changed)
        {
            tx_dat[1] = vr_y_l->value->lastVal;
            vr_y_l->value->newValueReady = false;
        }

        if (sw_l->value->newValueReady && sw_l->value->changed)
        {
            tx_dat[2] = sw_l->value->lastVal;
            sw_l->value->newValueReady = false;
        }

        // right joystick
        if (vr_x_r->value->newValueReady && vr_x_r->value->changed)
        {
            tx_dat[3] = vr_x_r->value->lastVal;
            vr_x_r->value->newValueReady = false;
        }

        if (vr_y_r->value->newValueReady && vr_y_r->value->changed)
        {
            tx_dat[4] = vr_y_r->value->lastVal;
            vr_y_r->value->newValueReady = false;
        }

        if (sw_r->value->newValueReady && sw_r->value->changed)
        {
            tx_dat[5] = sw_r->value->lastVal;
            sw_r->value->newValueReady = false;
        }

        if (batt->value->newValueReady && batt->value->changed)
        {
            ssd1306_clear(oled);
            ushort percent = psu_calculate_percent(psu, batt->value->lastVal);

            sprintf(print_out, "%d%%", percent);
            ssd1306_txt(oled, 0, 0, "Battery:");
            ssd1306_txt(oled, 128 - (strlen(print_out) * 7), 0, print_out);

            sprintf(print_out, "%d %d %d", psu->adc_min, psu->adc_max, batt->value->lastVal);
            ssd1306_txt(oled, 0, 16, print_out);

            batt->value->newValueReady = false;
        }

        nrf_transmit(nrf, tx_dat, PAYLOAD_SIZE);
        if (nrf_status_tx_data_sent(nrf))
            ssd1306_txt(oled, 121, 16, "+");
        else
            ssd1306_txt(oled, 121, 16, "-");

        ssd1306_display(oled);

        pause(100);
    }
}
