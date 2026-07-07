#ifndef __SX1278_H
#define __SX1278_H

#include "main.h"
#include "spi.h"
#include "gpio.h"
#include <stdint.h>

/* ================= GPIO Definitions ================= */

/* Change these if you use different pins */
#define SX1278_NSS_PORT     GPIOA
#define SX1278_NSS_PIN      GPIO_PIN_4

#define SX1278_RESET_PORT   GPIOB
#define SX1278_RESET_PIN    GPIO_PIN_0

/* ================= Registers ================= */

#define REG_FIFO                    0x00
#define REG_OP_MODE                 0x01
#define REG_FRF_MSB                 0x06
#define REG_FRF_MID                 0x07
#define REG_FRF_LSB                 0x08
#define REG_PA_CONFIG               0x09
#define REG_LNA                     0x0C
#define REG_FIFO_ADDR_PTR           0x0D
#define REG_FIFO_TX_BASE_ADDR       0x0E
#define REG_FIFO_RX_BASE_ADDR       0x0F
#define REG_FIFO_RX_CURRENT_ADDR    0x10
#define REG_IRQ_FLAGS               0x12
#define REG_RX_NB_BYTES             0x13
#define REG_PKT_SNR_VALUE           0x19
#define REG_PKT_RSSI_VALUE          0x1A
#define REG_MODEM_CONFIG_1          0x1D
#define REG_MODEM_CONFIG_2          0x1E
#define REG_PREAMBLE_MSB            0x20
#define REG_PREAMBLE_LSB            0x21
#define REG_PAYLOAD_LENGTH          0x22
#define REG_MODEM_CONFIG_3          0x26
#define REG_IRQ_FLAGS_MASK      0x11
#define REG_HOP_PERIOD          0x24
#define REG_DIO_MAPPING_2       0x41
#define REG_PA_DAC              0x4D
#define REG_SYNC_WORD               0x39
#define REG_DIO_MAPPING_1           0x40
#define REG_VERSION                 0x42

/* ================= Modes ================= */

#define MODE_LONG_RANGE_MODE        0x80

#define MODE_SLEEP                  0x00
#define MODE_STDBY                  0x01
#define MODE_TX                     0x03
#define MODE_RX_CONTINUOUS          0x05
#define MODE_RX_SINGLE              0x06

/* ================= IRQ Flags ================= */

#define IRQ_TX_DONE                 0x08
#define IRQ_RX_DONE                 0x40
#define IRQ_PAYLOAD_CRC_ERROR       0x20

/* ================= Public Functions ================= */

void SX1278_Reset(void);

void SX1278_WriteReg(uint8_t reg, uint8_t value);
uint8_t SX1278_ReadReg(uint8_t reg);

void SX1278_WriteBuffer(uint8_t reg, uint8_t *buffer, uint8_t length);
void SX1278_ReadBuffer(uint8_t reg, uint8_t *buffer, uint8_t length);

HAL_StatusTypeDef SX1278_Init(void);
HAL_StatusTypeDef SX1278_SetFrequency(uint32_t frequency);
HAL_StatusTypeDef SX1278_SendPacket(uint8_t *data, uint8_t length);

#endif
