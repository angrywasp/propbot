#ifdef DESKTOP
#include "./simulator.h"
#endif

#include "./SPI.h"
#include "./NRF24L01.h"
#include "./refs.h"
#include "serial_buffer.h"

const byte nrf_CONFIG = 0x00;
const byte nrf_EN_AA = 0x01;
const byte nrf_EN_RXADDR = 0x02;
const byte nrf_SETUP_AW = 0x03;
const byte nrf_SETUP_RETR = 0x04;
const byte nrf_RF_CH = 0x05;
const byte nrf_RF_SETUP = 0x06;
const byte nrf_STATUS = 0x07;
const byte nrf_OBSERVE_TX = 0x08;
const byte nrf_CD = 0x09;
const byte nrf_RX_ADDR_P0 = 0x0a;
const byte nrf_RX_ADDR_P1 = 0x0b;
const byte nrf_RX_ADDR_P2 = 0x0c;
const byte nrf_RX_ADDR_P3 = 0x0d;
const byte nrf_RX_ADDR_P4 = 0x0e;
const byte nrf_RX_ADDR_P5 = 0x0f;
const byte nrf_TX_ADDR = 0x10;
const byte nrf_RX_PW_P0 = 0x11;
const byte nrf_RX_PW_P1 = 0x12;
const byte nrf_RX_PW_P2 = 0x13;
const byte nrf_RX_PW_P3 = 0x14;
const byte nrf_RX_PW_P4 = 0x15;
const byte nrf_RX_PW_P5 = 0x16;
const byte nrf_FIFO_STATUS = 0x17;
const byte nrf_DYNPD = 0x1c;
const byte nrf_FEATURE = 0x1d;

static byte _nrf_R_REGISTER_byte(volatile nrf_context_t* cxt, byte reg)
{
    spi_select_chip(cxt->cs);

    spi_transfer(cxt->spi, reg);
    byte result = spi_transfer(cxt->spi, 0xff);

    spi_deselect_chip(cxt->cs);
    return result;
}

static void _nrf_R_REGISTER_data(volatile nrf_context_t* cxt, byte reg, byte* data, byte count)
{
    spi_select_chip(cxt->cs);

    spi_transfer(cxt->spi, reg);
    for (int i = 0; i < count; i++)
        data[i] = spi_transfer(cxt->spi, 0xff);
    
    spi_deselect_chip(cxt->cs);
}

static byte _nrf_W_REGISTER_byte(volatile nrf_context_t* cxt, byte reg, byte value)
{
    spi_select_chip(cxt->cs);

    spi_transfer(cxt->spi, 0b00100000 | reg);
    byte result = spi_transfer(cxt->spi, value);

    spi_deselect_chip(cxt->cs);
    return result;
}

static void _nrf_W_REGISTER_data(volatile nrf_context_t* cxt, byte reg, byte* data, byte count)
{
    spi_select_chip(cxt->cs);
    spi_transfer(cxt->spi, 0b00100000 | reg);
    
    for (int i = 0; i < count; i++)
        spi_transfer(cxt->spi, data[i]);

    spi_deselect_chip(cxt->cs);
}

nrf_context_t* nrf_init(int mosi, int miso, int clk, int cs, int ce)
{
    nrf_context_t* cxt = (nrf_context_t*)malloc(sizeof(nrf_context_t));
    spi_context_t* spi_context = spi_init(mosi, miso, clk, 1, 0);

    lo(ce);
    dir_out(ce);

    cxt->spi = spi_context;
    cxt->cs = cs;
    cxt->ce = ce;

    pause(1);

    nrf_wake_up(cxt);
    nrf_set_power_radio(cxt, 1);
    nrf_set_primary_mode(cxt, 1);

    return cxt;
}

void nrf_wake_up(volatile nrf_context_t* cxt)
{
    spi_wake_up(cxt->cs);
}

void nrf_enable(volatile nrf_context_t* cxt, bool on)
{
    out(cxt->ce, on);
}

byte nrf_set_power_radio(volatile nrf_context_t* cxt, bool on)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_CONFIG);
    if (on)
        value |= 0b00000010;
    else
        value &= !0b00000010;

    _nrf_W_REGISTER_byte(cxt, nrf_CONFIG, value);
    return _nrf_R_REGISTER_byte(cxt, nrf_CONFIG);
}

//mode: 1 = Primary Rx, 0 = Primary Tx  (and always power on and 2-byte crc) 
byte nrf_set_primary_mode(volatile nrf_context_t* cxt, byte mode)
{
    _nrf_W_REGISTER_byte(cxt, nrf_CONFIG, 0b00001110 + mode);
    return _nrf_R_REGISTER_byte(cxt, nrf_CONFIG);
}

//bits 0-5 represents pipes 0-5. pipe = 1 means enabled
byte nrf_enable_rx_pipes(volatile nrf_context_t* cxt, byte pipe)
{
    _nrf_W_REGISTER_byte(cxt, nrf_EN_RXADDR, pipe);
    return _nrf_R_REGISTER_byte(cxt, nrf_EN_RXADDR);
}

//112 or 113 is good
byte nrf_set_rf_channel(volatile nrf_context_t* cxt, byte channel)
{
    _nrf_W_REGISTER_byte(cxt, nrf_RF_CH, channel);
    return _nrf_R_REGISTER_byte(cxt, nrf_RF_CH);
}

byte nrf_set_shockburst(volatile nrf_context_t* cxt, bool on)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_EN_AA);
    if (on)
        value |= 0b00111111;
    else
        value &= !0b00111111;

    _nrf_W_REGISTER_byte(cxt, nrf_EN_AA, value);
    return _nrf_R_REGISTER_byte(cxt, nrf_EN_AA);
}

byte nrf_set_transmitter_power(volatile nrf_context_t* cxt, sbyte dB)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_RF_SETUP) & !0b00000110;

    switch (dB)
    {
        case -18:
            value |= 0b00000000;
            break;
        case -12:
            value |= 0b00000010;
            break;
        case -6:
            value |= 0b00000100;
            break;
        case 0:
            value |= 0b00000110;
            break;
    }

    _nrf_W_REGISTER_byte(cxt, nrf_RF_SETUP, value);
    return _nrf_R_REGISTER_byte(cxt, nrf_RF_SETUP);
}

//rate: 1 = 2MB/s, 0 = 1MB/s
byte nrf_set_data_rate(volatile nrf_context_t* cxt, byte rate)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_RF_SETUP) & !0b00001000;
    _nrf_W_REGISTER_byte(cxt, nrf_RF_SETUP, value |= (rate & 1) << 3);
    return _nrf_R_REGISTER_byte(cxt, nrf_RF_SETUP);
}

//Set payload width in specified pipe - max 32 bytes. 0 = pipe not used 
byte nrf_set_payload_width(volatile nrf_context_t* cxt, byte pipe, byte width)
{
    _nrf_W_REGISTER_byte(cxt, nrf_RX_PW_P0 + pipe, width);
    return _nrf_R_REGISTER_byte(cxt, nrf_RX_PW_P0 + pipe);
}

/*
Write the unique address to be used by the rx radio - for a given pipe                      
address width is set to 3-5 bytes in SETUP_AW, use this value                                                                                                                      
pipes 0 and 1 takes full address width, but pipes 2-5 only uses the LSB, don't know what 
happens when attempting to write more address bytes in this case 
*/
byte nrf_write_rx_pipe_address(volatile nrf_context_t* cxt, byte pipe, byte* address)
{
    byte addr_width = _nrf_R_REGISTER_byte(cxt, nrf_SETUP_AW);
    //printf("nrf_write_rx_pipe_address: %d\n", addr_width);
    _nrf_W_REGISTER_data(cxt, nrf_RX_ADDR_P0 + pipe, address, addr_width);
    return addr_width;
}

/*
Write the unique address to be used by the tx radio                      
address width is set to 3-5 bytes in SETUP_AW, use this value                                                                                                                                                                
when using auto-ACK, this address should be the same as Pipe0 rx address
*/
byte nrf_write_tx_pipe_address(volatile nrf_context_t* cxt, byte* address)
{
    byte addr_width = _nrf_R_REGISTER_byte(cxt, nrf_SETUP_AW);
    //printf("nrf_write_tx_pipe_address: %d\n", addr_width);
    _nrf_W_REGISTER_data(cxt, nrf_TX_ADDR, address, addr_width);
    return addr_width;
}

byte nrf_flush_rx(volatile nrf_context_t* cxt)
{
    spi_select_chip(cxt->cs);
    byte value = spi_transfer(cxt->spi, 0b11100010);
    spi_deselect_chip(cxt->cs);
    return value;
}

byte nrf_flush_tx(volatile nrf_context_t* cxt)
{
    spi_select_chip(cxt->cs);
    byte value = spi_transfer(cxt->spi, 0b11100001);
    spi_deselect_chip(cxt->cs);
    return value;
}

byte nrf_read_and_clear_interrupts(volatile nrf_context_t* cxt)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_STATUS);
    _nrf_W_REGISTER_byte(cxt, nrf_STATUS, value);
    return value;
}

void nrf_transmit(volatile nrf_context_t* cxt, byte* data, byte count)
{
    spi_select_chip(cxt->cs);

    spi_transfer(cxt->spi, 0b10100000);
    for (int i = 0; i < count; i++)
        spi_transfer(cxt->spi, data[i]);
    
    spi_deselect_chip(cxt->cs);
}

void nrf_receive(volatile nrf_context_t* cxt, byte* data, byte count)
{
    spi_select_chip(cxt->cs);

    spi_transfer(cxt->spi, 0b01100001);
    for (int i = 0; i < count; i++)
        data[i] = spi_transfer(cxt->spi, 0xff);
    
    spi_deselect_chip(cxt->cs);
}

bool nrf_is_payload_rx(volatile nrf_context_t* cxt)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_STATUS & 0b01000000) >> 6;
    return value;
}

bool nrf_is_payload_pipe(volatile nrf_context_t* cxt)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_STATUS & 0b00001110) >> 1;
    return value;
}

bool nrf_is_data_sent(volatile nrf_context_t* cxt)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_STATUS & 0b00100000) >> 5;
    return value;
}

bool nrf_is_max_retries(volatile nrf_context_t* cxt)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_STATUS & 0b00010000) >> 4;
    return value;
}

bool nrf_is_tx_buffer_full(volatile nrf_context_t* cxt)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_STATUS & 0b00000001);
    return value;
}