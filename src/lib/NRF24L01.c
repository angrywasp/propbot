#include "./NRF24L01.h"
#include "./refs.h"

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

static byte _spi_transfer(volatile nrf_context_t* cxt, byte value)
{
    byte rx = value;

    for (int i = 0; i < 8; i++)
    {
        if (rx & 0x80)
            hi(cxt->mosi);
        else
            lo(cxt->mosi);

        rx <<= 1;

        usleep(1);
        hi(cxt->clk);
        usleep(1);
        rx |= in(cxt->miso);
        lo(cxt->clk);
    }

    usleep(100);

    return rx;
}


static byte _nrf_R_REGISTER_byte(volatile nrf_context_t* cxt, byte reg)
{
    lo(cxt->cs);

    _spi_transfer(cxt, 0b00000000 | reg);
    byte result = _spi_transfer(cxt, 0xff);

    hi(cxt->cs);
    return result;
}

static void _nrf_R_REGISTER_data(volatile nrf_context_t* cxt, byte reg, byte* data, byte count)
{
    lo(cxt->cs);

    _spi_transfer(cxt, reg);
    for (int i = 0; i < count; i++)
        data[i] = _spi_transfer(cxt, 0xff);
    
    hi(cxt->cs);
}

static byte _nrf_W_REGISTER_byte(volatile nrf_context_t* cxt, byte reg, byte value)
{
    lo(cxt->cs);

    _spi_transfer(cxt, 0b00100000 | reg);
    byte result = _spi_transfer(cxt, value);

    hi(cxt->cs);
    return result;
}

static void _nrf_W_REGISTER_data(volatile nrf_context_t* cxt, byte reg, byte* data, byte count)
{
    lo(cxt->cs);
    _spi_transfer(cxt, 0b00100000 | reg);
    
    for (int i = 0; i < count; i++)
        _spi_transfer(cxt, data[i]);

    hi(cxt->cs);
}

nrf_context_t* nrf_init(int mosi, int miso, int clk, int cs, int ce)
{
    nrf_context_t* cxt = (nrf_context_t*)malloc(sizeof(nrf_context_t));

    lo(mosi);
    lo(ce);
    lo(clk);

    dir_out(mosi);
    dir_in(miso);
    dir_out(clk);
    dir_out(ce);

    cxt->mosi = mosi;
    cxt->miso = miso;
    cxt->clk = clk;
    cxt->cs = cs;
    cxt->ce = ce;

    pause(1);
    
    nrf_set_power_radio(cxt, 1);
    nrf_wake_up(cxt);
    
    pause(1);

    return cxt;
}

void nrf_wake_up(volatile nrf_context_t* cxt)
{
    hi(cxt->cs);
    lo(cxt->cs);
    hi(cxt->cs);
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

    _nrf_W_REGISTER_data(cxt, nrf_TX_ADDR, address, addr_width);
    return addr_width;
}

byte nrf_flush_rx(volatile nrf_context_t* cxt)
{
    lo(cxt->cs);
    byte value = _spi_transfer(cxt, 0b11100010);
    hi(cxt->cs);
    return value;
}

byte nrf_flush_tx(volatile nrf_context_t* cxt)
{
    lo(cxt->cs);
    byte value = _spi_transfer(cxt, 0b11100001);
    hi(cxt->cs);
    return value;
}

//writing back the same value clears the interrupt bits
byte nrf_read_and_clear_interrupts(volatile nrf_context_t* cxt)
{
    byte value = _nrf_R_REGISTER_byte(cxt, nrf_STATUS);
    _nrf_W_REGISTER_byte(cxt, nrf_STATUS, value);
    return value;
}

void nrf_transmit(volatile nrf_context_t* cxt, byte* data, byte count)
{
    nrf_flush_tx(cxt);
    nrf_read_and_clear_interrupts(cxt);

    lo(cxt->cs);

    _spi_transfer(cxt, 0b10100000);
    for (int i = 0; i < count; i++)
        _spi_transfer(cxt, data[i]);
    8
    hi(cxt->cs);

    hi(cxt->ce);
    usleep(50);
    lo(cxt->ce);
    usleep(50);
}

void nrf_receive(volatile nrf_context_t* cxt, byte* data, byte count)
{
    lo(cxt->cs);

    _spi_transfer(cxt, 0b01100001);
    for (int i = 0; i < count; i++)
        data[i] = _spi_transfer(cxt, 0xff);
    
    hi(cxt->cs);

    nrf_read_and_clear_interrupts(cxt);
}

byte nrf_read_status(volatile nrf_context_t* cxt)
{
    return _nrf_R_REGISTER_byte(cxt, nrf_STATUS);
}

byte nrf_read_fifo_status(volatile nrf_context_t* cxt)
{
    return _nrf_R_REGISTER_byte(cxt, nrf_FIFO_STATUS);
}

bool nrf_status_tx_fifo_full(volatile nrf_context_t* cxt)
{
    return (_nrf_R_REGISTER_byte(cxt, nrf_STATUS) >> 0) & 0x01;
}

bool nrf_status_tx_data_sent(volatile nrf_context_t* cxt)
{
    return (_nrf_R_REGISTER_byte(cxt, nrf_STATUS) >> 5) & 0x01;
}

bool nrf_status_rx_data_ready(volatile nrf_context_t* cxt)
{
    return (_nrf_R_REGISTER_byte(cxt, nrf_STATUS) >> 6) & 0x01;
}

bool nrf_fifo_status_rx_empty(volatile nrf_context_t* cxt)
{
    return (_nrf_R_REGISTER_byte(cxt, nrf_FIFO_STATUS) >> 0) & 0x01;
}

bool nrf_fifo_status_rx_full(volatile nrf_context_t* cxt)
{
    return (_nrf_R_REGISTER_byte(cxt, nrf_FIFO_STATUS) >> 1) & 0x01;
}

bool nrf_fifo_status_tx_empty(volatile nrf_context_t* cxt)
{
    return (_nrf_R_REGISTER_byte(cxt, nrf_FIFO_STATUS) >> 4) & 0x01;
}

bool nrf_fifo_status_tx_full(volatile nrf_context_t* cxt)
{
    return (_nrf_R_REGISTER_byte(cxt, nrf_FIFO_STATUS) >> 5) & 0x01;
}

byte nrf_read_payload_width(volatile nrf_context_t* cxt)
{
    return _nrf_R_REGISTER_byte(cxt, 0b01100000);
}
