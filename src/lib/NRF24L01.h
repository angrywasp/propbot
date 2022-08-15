#ifndef __nrf_H
#define __nrf_H

/*
       +----------------+                                                           PrimaryTX
       |                +                                          Communication    +-------+
       |   nrf+   10uF            Propeller                   link topology    | Pipe5 |    PrimaryTX
       |  +---------+   +            +----------+                  in a one-many    |       |    +-------+
3.3V+--+--+VCC |GND +---+-+GND       |          |                  configuration    | Pipe4 |    | Pipe5 |    PrimaryTX
          |    |    |                |          |                                   |       |    |       |    +-------+
     +----+CSN |CE  +----------------+ PINce    |                                   | Pipe3 |    | Pipe4 |    | Pipe5 |    PrimaryTX
     |    |    |    |                |          |                    HUB            |       |    |       |    |       |    +-------+
     | +--+MOSI|SCK +----------------+ PINclk   |                  PrimaryRX        | Pipe2 |    | Pipe3 |    | Pipe4 |    | Pipe5 |
     | |  |    |    |                |          |                  +-------+        |       |    |       |    |       |    |       |
     | |  |IRQ |MISO+----------------+ PINmiso  |                  | Pipe5 |        | Pipe1 |    | Pipe2 |    | Pipe3 |    | Pipe4 |
     | |  +---------+                |          |                  |       |        |       |    |       |    |       |    |       |
     | |                             |          |                  | Pipe4 |        | Pipe0 +--+ | Pipe1 | ++ | Pipe2 | ++ | Pipe3 | +--+  +
     | |                             |          |                  |       |        |       |    |       |    |       |    |       |       |
     | +-----------------------------+ PINmosi  |                  | Pipe3 +-<<----+ TX   |    | Pipe0 +--+ | Pipe1 | ++ | Pipe2 | +--+  |
     |                               |          |                  |       |        +-------+    |       |    |       |    |       |       |
     +-------------------------------+ PINcs    |                  | Pipe2 +-<<------------------+  TX   | ++ | Pipe0 +--+ | Pipe1 | +--+  |
                                     |          |                  |       |                     +-------+    |       |    |       |       |
                                                                   | Pipe1 +-<<-------------------------------+  TX   |    | Pipe0 +----+  |
  Connection between nrf+ module and Propeller                |       |                                  +-------+    |       |       |
                                                                   | Pipe0 +-<<--------------------------------------------+  TX   |       |
                                                                   |       |                                               +-------+       |
                                                                   |   TX  +->>------------------------------------------------------------+
                                                                   +-------+   Pipe0 of satellites must be same address as TX of hub
 About nrf+
---------------

The nrf is controlled using twelve instructions that can be sent over the SPI bus. An instruction must be sent using this process:
     Use SPI Mode 0
  1  Set CSN low.
  2  Send the instruction byte.
  3  If the instruction has arguments, send the argument bytes. If the instruction returns data, send NOP bytes (0xFF) and read in the responses.
  4  Set CSN high.

Before reading/writing packets, put the radio into Standby-I mode by setting CE low.
To enter Tx Mode from Standby-I(PTX) (i.e. to transmit), pulse CE high for at least 10 uS.
To enter into Rx Mode from Standby-I(PRX) hold the ce high.

Set the radio to either a primary receiver or primary transmitter, depending on it's role. Typically the hub/central  should be primary receiver (PRX).


PACKETS
===========================================================================================================================================
Enhanced Shockburst Mode applies a packet structure as follows:
_____________________________________________________________________________________________
PREAMBLE(1byte) | ADDRESS(3-5bytes) | PACKET CNTL(9bit) | PAYLOAD(0-32bytes) | CRC(1-2bytes)|
---------------------------------------------------------------------------------------------
- with the PACKET CNTL field having this structure:
______________________________________________________
PAYLOAD Length(6bit) | PacketID(2bit) | NO_ACK(1bit) |
------------------------------------------------------

FIFOs
There are two packet queues, or FIFOs, in the radio, one for received packets (the Rx FIFO) and one for packets to transmit (the Tx FIFO0.
Each FIFO can hold up to three packets. If a transmission fails then the packet is not cleared from the Tx FIFO, it has to be removed using
the FLUSH_TX instruction. The Rx FIFO is different from the Rx pipes: a pipe refers to one of the up to six addresses by which
the radio can be addressed, whereas the Rx FIFO is the 3-packet queue in which received packets are stored until they are read out of the radio.
All six Rx pipes put data into the same 3-packet Rx FIFO.

When the Tx FIFO is full, adding packets to the FIFO pushes the oldest packet out of the queue. When the Rx FIFO is full, new packets are
dropped until a space in the FIFO becomes available.

The Tx FIFO is not a true queue. If a receiver is using the ack payload feature then the ack payload is stored in the Tx FIFO.
The ack payload is only transmitted in an ack packet though, even if it’s at the head of the FIFO during a transmission. If the ack payload
is not at the head of the FIFO and an ack packet is transmitted, then the ack payload is removed from whatever FIFO slot it occupies.



        nrf+ REGISTER OVERVIEW
        =============================================================================================================================================
        $00   CONFIG          %00000100  rec:%0000111x                          Configure interrupts, CRC, power, and Tx/Rx status.
        $01   EN_AA           %00111111  enable all pipes ESB mode              Enable and disable Enhanced Shockburst™ on individual Rx pipes.
        $02   EN_RXADDR       %00000011  enable pipes 0 and 1                   Enable and disable the Rx pipes.
        $03   SETUP_AW        %00000011  enable 5byte address witht             Set the address width.
        $04   SETUP_RETR      %00000011  delay 250uS, 5 retries                 Configure the retry delay and number of retries that the radio will do
        $05   RF_CH           %00000010  rec:%01110000 +-3 (112+-3)             Set the RF channel on which the radio broadcasts.
        $06   RF_SETUP        %00001111  max power and rate                     Configure the radio’s on-air data rate, output power, and LNA gain.
        $07   STATUS          %00001110  status after reset                     Interrupt status bits, Tx FIFO full bit, and the number of the pipe that received a packet.
        $08   OBSERVE_TX      %00000000  read only                              Count of lost and re-transmitted packets.
        $09   CD              %00000000  read only                              Carrier detect bit.
        $0A   RX_ADDR_P0      $E7E7E7E7E7     avoid regular bit patterns        Set the address for Rx pipe 0 default 5byte wide
        $0B   RX_ADDR_P1      $C2C2C2C2C2     avoid regular bit patterns        Set the address for Rx pipe 1 default 5byte wide
        $0C   RX_ADDR_P2      $C3                                               Set the address for Rx pipe 2
        $0D   RX_ADDR_P3      $C4                                               Set the address for Rx pipe 3
        $0E   RX_ADDR_P4      $C5                                               Set the address for Rx pipe 4
        $0F   RX_ADDR_P5      $C6                                               Set the address for Rx pipe 5
        $10   TX_ADDR         $E7E7E7E7E7                                       Set the destination address for transmitted packets.
        $11   RX_PW_P0        %00000000  set to max 32                          Set the static payload width on Rx pipe 0
        $12   RX_PW_P1        %00000000  set to max 32                          Set the static payload width on Rx pipe 1
        $13   RX_PW_P2        %00000000  set to max 32                          Set the static payload width on Rx pipe 2
        $14   RX_PW_P3        %00000000  set to max 32                          Set the static payload width on Rx pipe 3
        $15   RX_PW_P4        %00000000  set to max 32                          Set the static payload width on Rx pipe 4
        $16   RX_PW_P5        %00000000  set to max 32                          Set the static payload width on Rx pipe 5
        $17   FIFO_STATUS     %00010001  read only                              Auto-retransmit status, Tx FIFO full/empty, Rx FIFO full/empty.

         na   ACK_PLD         The 32byte payload to send with ack packets, if ack packet payloads are enabled (written to with the W_ACK_PAYLOAD instruction).
         na   TX_PLD          The 32byte Tx FIFO (written to with the W_TX_PAYLOAD and W_TX_PAYLOAD_NO_ACK instructions).
         na   RX_PLD          The 32byte Rx FIFO (read from with the R_RX_PAYLOAD instruction)

        $1C   DYNPD           %00000000  have to do the ACTIVE cmd first        Enable or disable the dynamic payload calculation feature on the Rx pipes.
        $1D   FEATURE         %00000000  have to do the ACTIVE cmd first        Enable or disable the dynamic payload, ack payload, and selective ack features
*/

#include "./refs.h"

extern const byte nrf_CONFIG;
extern const byte nrf_EN_AA;
extern const byte nrf_EN_RXADDR;
extern const byte nrf_SETUP_AW;
extern const byte nrf_SETUP_RETR;
extern const byte nrf_RF_CH;
extern const byte nrf_RF_SETUP;
extern const byte nrf_STATUS;
extern const byte nrf_OBSERVE_TX;
extern const byte nrf_CD;
extern const byte nrf_RX_ADDR_P0;
extern const byte nrf_RX_ADDR_P1;
extern const byte nrf_RX_ADDR_P2;
extern const byte nrf_RX_ADDR_P3;
extern const byte nrf_RX_ADDR_P4;
extern const byte nrf_RX_ADDR_P5;
extern const byte nrf_TX_ADDR;
extern const byte nrf_RX_PW_P0;
extern const byte nrf_RX_PW_P1;
extern const byte nrf_RX_PW_P2;
extern const byte nrf_RX_PW_P3;
extern const byte nrf_RX_PW_P4;
extern const byte nrf_RX_PW_P5;
extern const byte nrf_FIFO_STATUS;
extern const byte nrf_DYNPD;
extern const byte nrf_FEATURE;

typedef struct
{
    volatile int mosi;
    volatile int miso;
    volatile int clk;
    volatile int cs;
    volatile int ce;
} nrf_context_t;

nrf_context_t* nrf_init(int mosi, int miso, int clk, int cs, int ce);
void nrf_wake_up(volatile nrf_context_t* cxt);
byte nrf_set_power_radio(volatile nrf_context_t* cxt, bool on);
byte nrf_set_primary_mode(volatile nrf_context_t* cxt, byte mode);
byte nrf_enable_rx_pipes(volatile nrf_context_t* cxt, byte pipe);
byte nrf_set_rf_channel(volatile nrf_context_t* cxt, byte channel);
byte nrf_set_shockburst(volatile nrf_context_t* cxt, bool on);
byte nrf_set_transmitter_power(volatile nrf_context_t* cxt, sbyte dB);
byte nrf_set_data_rate(volatile nrf_context_t* cxt, byte rate);
byte nrf_set_payload_width(volatile nrf_context_t* cxt, byte pipe, byte width);
byte nrf_write_rx_pipe_address(volatile nrf_context_t* cxt, byte pipe, byte* address);
byte nrf_write_tx_pipe_address(volatile nrf_context_t* cxt, byte* address);
byte nrf_flush_rx(volatile nrf_context_t* cxt);
byte nrf_flush_tx(volatile nrf_context_t* cxt);
byte nrf_read_and_clear_interrupts(volatile nrf_context_t* cxt);

byte nrf_read_status(volatile nrf_context_t* cxt);
byte nrf_read_fifo_status(volatile nrf_context_t* cxt);

bool nrf_status_tx_fifo_full(volatile nrf_context_t* cxt);
bool nrf_status_tx_data_sent(volatile nrf_context_t* cxt);
bool nrf_status_rx_data_ready(volatile nrf_context_t* cxt);

bool nrf_fifo_status_rx_empty(volatile nrf_context_t* cxt);
bool nrf_fifo_status_rx_full(volatile nrf_context_t* cxt);
bool nrf_fifo_status_tx_empty(volatile nrf_context_t* cxt);
bool nrf_fifo_status_tx_full(volatile nrf_context_t* cxt);

void nrf_transmit(volatile nrf_context_t* cxt, byte* data, byte count);
void nrf_receive(volatile nrf_context_t* cxt, byte* data, byte count);

byte nrf_read_payload_width(volatile nrf_context_t* cxt);

#endif