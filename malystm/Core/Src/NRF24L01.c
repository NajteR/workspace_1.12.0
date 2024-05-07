/*
 * NRF24L01.c
 *
 *  Created on: Jul 20, 2023
 *      Author: Marcin Paszczak
 */


#include "stm32f1xx_hal.h"
#include "NRF24L01.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

// NRF CE PIN (DI) Chip enable - activates RX or TX mode
#define NRF24_CE_PORT	CE_GPIO_Port
#define NRF24_CE_PIN	CE_Pin

// NRF CSN PIN (DI) SPI Chip Select
#define NRF24_CSN_PORT	CSN_GPIO_Port
#define NRF24_CSN_PIN	CSN_Pin

//Function to enable device
void CE_Enable ()
{
	HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_SET);
}

//Function to enable device
void CE_Disable ()
{
	HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_RESET);
}

//Function to chip select
void CSN_Select ()
{
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET);
}

//Function to chip unselect
void CSN_UnSelect ()
{
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);
}

//Write single bytes to the register
void nrf_WriteRegSingle (uint8_t Reg, uint8_t Data)
{
	uint8_t buf[2];
	// Write command and status registers (001A AAAA, AAAAA - 5 bit Register Map Address)
	buf[0] = Reg|1<<5;
	buf[1] = Data;
	// Select the device
	CSN_Select();

	HAL_SPI_Transmit(&hspi1, buf, 2, 1000);

	// Release the device
	CSN_UnSelect();
}

//Write multiple bytes to  the register
void nrf_WriteRegMulti (uint8_t Reg, uint8_t *data, uint16_t size)
{
	uint8_t buf[2];
	// Write command and status registers (001A AAAA, AAAAA - 5 bit Register Map Address)
	buf[0] = Reg|1<<5;

	// Select the device
	CSN_Select();

	HAL_SPI_Transmit(&hspi1, buf, 1, 100);
	HAL_SPI_Transmit(&hspi1, data, size, 1000);

	// Release the device
	CSN_UnSelect();
}

//Read single byte from the register
uint8_t nrf_ReadRegSingle (uint8_t Reg)
{
	uint8_t data = 0;
	// Select the device
	CSN_Select();

	HAL_SPI_Transmit(&hspi1, &Reg, 1, 100);
	HAL_SPI_Receive(&hspi1, &data, 1, 100);

	// Release the device
	CSN_UnSelect();

	return data;
}

//Read multiple bytes from the register
void nrf_ReadRegMulti (uint8_t Reg, uint8_t *data, uint16_t size)
{

	// Select the device
	CSN_Select();

	HAL_SPI_Transmit(&hspi1, &Reg, 1, 100);
	HAL_SPI_Receive(&hspi1, data, size, 1000);

	// Release the device
	CSN_UnSelect();
}

//Send the command to the NRF
void nrf_SendCmd(uint8_t cmd)
{
	// Select the device
	CSN_Select();

	HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);

	// Release the device
	CSN_UnSelect();
}
//reset
void nrf_reset(uint8_t REG)
{
	if (REG == STATUS)
	{
		nrf_WriteRegSingle(STATUS, 0x00);
	}

	else if (REG == FIFO_STATUS)
	{
		nrf_WriteRegSingle(FIFO_STATUS, 0x11);
	}

	else {
	nrf_WriteRegSingle(CONFIG, 0x08);
	nrf_WriteRegSingle(EN_AA, 0x3F);
	nrf_WriteRegSingle(EN_RXADDR, 0x03);
	nrf_WriteRegSingle(SETUP_AW, 0x03);
	nrf_WriteRegSingle(SETUP_RETR, 0x03);
	nrf_WriteRegSingle(RF_CH, 0x02);
	nrf_WriteRegSingle(RF_SETUP, 0x0E);
	nrf_WriteRegSingle(STATUS, 0x00);
	nrf_WriteRegSingle(OBSERVE_TX, 0x00);
	nrf_WriteRegSingle(CD, 0x00);
	uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	nrf_WriteRegMulti(RX_ADDR_P0, rx_addr_p0_def, 5);
	uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
	nrf_WriteRegMulti(RX_ADDR_P1, rx_addr_p1_def, 5);
	nrf_WriteRegSingle(RX_ADDR_P2, 0xC3);
	nrf_WriteRegSingle(RX_ADDR_P3, 0xC4);
	nrf_WriteRegSingle(RX_ADDR_P4, 0xC5);
	nrf_WriteRegSingle(RX_ADDR_P5, 0xC6);
	uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	nrf_WriteRegMulti(TX_ADDR, tx_addr_def, 5);
	nrf_WriteRegSingle(RX_PW_P0, 0);
	nrf_WriteRegSingle(RX_PW_P1, 0);
	nrf_WriteRegSingle(RX_PW_P2, 0);
	nrf_WriteRegSingle(RX_PW_P3, 0);
	nrf_WriteRegSingle(RX_PW_P4, 0);
	nrf_WriteRegSingle(RX_PW_P5, 0);
	nrf_WriteRegSingle(FIFO_STATUS, 0x11);
	nrf_WriteRegSingle(DYNPD, 0);
	nrf_WriteRegSingle(FEATURE, 0);
	}
}

//initialize NRF24L01
void nrf_Init()
{
	//disable before config
	CE_Disable();

	nrf_reset(0);

	nrf_WriteRegSingle(CONFIG, 0); // to do later

	nrf_WriteRegSingle(EN_AA, 0); //No ACK

	nrf_WriteRegSingle(EN_RXADDR, 0); // Disabling data pipe

	nrf_WriteRegSingle(SETUP_AW, 0x03); // 5 Bytes for the TX/RX address

	nrf_WriteRegSingle(SETUP_RETR, 0); // No retransmission

	nrf_WriteRegSingle(RF_CH, 0); //to do later

	nrf_WriteRegSingle(RF_SETUP, 0x0E); // Power = 0dBm, data rate = 2Mbps

	//nrf_WriteRegSingle(CONFIG, 0);

	//enable after config
	CE_Enable();


}

// Transmission mode

void nrf_TxMode (uint8_t *Address, uint8_t channel)
{
	//disable before config
	CE_Disable();


	//channel select
	nrf_WriteRegSingle(RF_CH, channel);

	//set TX address
	nrf_WriteRegMulti(TX_ADDR, Address, 5);

	//device power up
	uint8_t config= nrf_ReadRegSingle(CONFIG);
	config = config & (0xF2);
	nrf_WriteRegSingle(CONFIG, config);

	//enable after config
	CE_Enable();

}

// Send data function

uint8_t nrf_Transmit (uint8_t *data)
{
	uint8_t cmdpayload = 0;
	// device select
	CSN_Select();

	//payload cmd
	cmdpayload = W_TX_PAYLOAD;
	HAL_SPI_Transmit(&hspi1, &cmdpayload, 1, 100);

	//send data
	HAL_SPI_Transmit(&hspi1, data, 32, 1000);

	// Release the device
	CSN_UnSelect();

	HAL_Delay(1);

	uint8_t fifostat = nrf_ReadRegSingle(FIFO_STATUS);

	if ((fifostat&(1<<4)) && (!(fifostat&(1<<3))))
	{
		nrf_SendCmd(FLUSH_TX);
		nrf_reset(FIFO_STATUS);
		return 1;
	}
	return 0;
}
