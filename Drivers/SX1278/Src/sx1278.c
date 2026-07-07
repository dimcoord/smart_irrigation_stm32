#include "sx1278.h"

/* Reset the SX1278 */
void SX1278_Reset(void)
{
    HAL_GPIO_WritePin(SX1278_RESET_PORT, SX1278_RESET_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(SX1278_RESET_PORT, SX1278_RESET_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
}

/* Write one register */
void SX1278_WriteReg(uint8_t reg, uint8_t value)
{
    uint8_t tx[2];

    tx[0] = reg | 0x80;    // MSB = 1 -> Write
    tx[1] = value;

    HAL_GPIO_WritePin(SX1278_NSS_PORT, SX1278_NSS_PIN, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, tx, 2, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(SX1278_NSS_PORT, SX1278_NSS_PIN, GPIO_PIN_SET);
}

/* Read one register */
uint8_t SX1278_ReadReg(uint8_t reg)
{
    uint8_t tx[2];
    uint8_t rx[2];

    tx[0] = reg & 0x7F;    // MSB = 0 -> Read
    tx[1] = 0x00;

    HAL_GPIO_WritePin(SX1278_NSS_PORT, SX1278_NSS_PIN, GPIO_PIN_RESET);

    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(SX1278_NSS_PORT, SX1278_NSS_PIN, GPIO_PIN_SET);

    return rx[1];
}

/* Write multiple bytes */
void SX1278_WriteBuffer(uint8_t reg, uint8_t *buffer, uint8_t length)
{
    HAL_GPIO_WritePin(SX1278_NSS_PORT, SX1278_NSS_PIN, GPIO_PIN_RESET);

    reg |= 0x80;
    HAL_SPI_Transmit(&hspi1, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, buffer, length, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(SX1278_NSS_PORT, SX1278_NSS_PIN, GPIO_PIN_SET);
}

/* Read multiple bytes */
void SX1278_ReadBuffer(uint8_t reg, uint8_t *buffer, uint8_t length)
{
    reg &= 0x7F;

    HAL_GPIO_WritePin(SX1278_NSS_PORT, SX1278_NSS_PIN, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, buffer, length, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(SX1278_NSS_PORT, SX1278_NSS_PIN, GPIO_PIN_SET);
}

HAL_StatusTypeDef SX1278_SetFrequency(uint32_t frequency)
{
    uint64_t frf;

    /* FRF = (frequency << 19) / 32MHz */
    frf = ((uint64_t)frequency << 19) / 32000000;

    SX1278_WriteReg(REG_FRF_MSB, (uint8_t)(frf >> 16));
    SX1278_WriteReg(REG_FRF_MID, (uint8_t)(frf >> 8));
    SX1278_WriteReg(REG_FRF_LSB, (uint8_t)(frf));

    return HAL_OK;
}

HAL_StatusTypeDef SX1278_Init(void)
{
	SX1278_Reset();
	HAL_Delay(20);

	if (SX1278_ReadReg(REG_VERSION) != 0x12)
	    return HAL_ERROR;

    /* Sleep */
	SX1278_WriteReg(REG_OP_MODE, 0x80);   // LoRa Sleep
	HAL_Delay(10);

    /* Standby */
	SX1278_WriteReg(REG_OP_MODE, 0x81);   // LoRa Standby
	HAL_Delay(10);

    /* 433 MHz */
    SX1278_SetFrequency(433000000);

    /* FIFO base addresses */
    SX1278_WriteReg(REG_FIFO_TX_BASE_ADDR, 0x00);
    SX1278_WriteReg(REG_FIFO_RX_BASE_ADDR, 0x00);

    /* LNA boost */
    SX1278_WriteReg(REG_LNA, 0x23);

    /* Automatic AGC */
    SX1278_WriteReg(REG_MODEM_CONFIG_3, 0x04);

    /* Bandwidth = 125kHz
       Coding Rate = 4/5 */
    SX1278_WriteReg(REG_MODEM_CONFIG_1, 0x72);

    /* SF7
       CRC ON */
    SX1278_WriteReg(REG_MODEM_CONFIG_2, 0x74);

    /* Preamble = 8 */
    SX1278_WriteReg(REG_PREAMBLE_MSB, 0x00);
    SX1278_WriteReg(REG_PREAMBLE_LSB, 0x08);

    /* Sync Word */
    SX1278_WriteReg(REG_SYNC_WORD, 0x12);

    /* PA_BOOST
       17 dBm */
    SX1278_WriteReg(REG_PA_CONFIG, 0x8F);

    /* Enable +17 dBm PA output */
    SX1278_WriteReg(REG_PA_DAC, 0x84);

    /* Disable frequency hopping */
    SX1278_WriteReg(REG_HOP_PERIOD, 0x00);

    return HAL_OK;
}

HAL_StatusTypeDef SX1278_SendPacket(uint8_t *data, uint8_t length)
{
    uint8_t irqFlags;

    /* Go to Standby */
    SX1278_WriteReg(REG_OP_MODE,
        MODE_LONG_RANGE_MODE | MODE_STDBY);

    /* Clear all IRQ flags */
    SX1278_WriteReg(REG_IRQ_FLAGS, 0xFF);

    /* Reset FIFO pointer to TX base (0x00) */
    SX1278_WriteReg(REG_FIFO_ADDR_PTR, 0x00);

    /* Write payload into FIFO */
    SX1278_WriteBuffer(REG_FIFO, data, length);

    /* Payload length */
    SX1278_WriteReg(REG_PAYLOAD_LENGTH, length);

    /* Enter TX mode */
    SX1278_WriteReg(REG_OP_MODE,
        MODE_LONG_RANGE_MODE | MODE_TX);

    /* Wait until TxDone */
    do
    {
        irqFlags = SX1278_ReadReg(REG_IRQ_FLAGS);
    }
    while ((irqFlags & IRQ_TX_DONE) == 0);

    /* Clear TxDone */
    SX1278_WriteReg(REG_IRQ_FLAGS, IRQ_TX_DONE);

    /* Return to Standby */
    SX1278_WriteReg(REG_OP_MODE,
        MODE_LONG_RANGE_MODE | MODE_STDBY);

    return HAL_OK;
}
