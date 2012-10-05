/*
 * SD_HaLayer.c
 *
 *  Created on: 23.09.2010
 *      Author: Sebastian
 */

#include "stm32f10x.h"
#include "D_HaLayer.h"


/*----------------------------------------------------------------------------*/
/**
  * @brief  Initizilizes SPI Bus for Communication
  * @param  None
  * @retval None
  */
void D_SPI_Init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  /* SD_SPI_CS_GPIO, SD_SPI_MOSI_GPIO, SD_SPI_MISO_GPIO, SD_SPI_DETECT_GPIO
       and SD_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(D_CS_GPIO_CLK | D_SPI_MOSI_GPIO_CLK | D_SPI_MISO_GPIO_CLK |
                         D_SPI_SCK_GPIO_CLK, ENABLE);

  /* D_SPI Periph clock enable */
  RCC_APB1PeriphClockCmd(D_SPI_CLK, ENABLE);


  /* Configure D_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = D_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(D_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* Configure D_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = D_SPI_MISO_PIN;
  GPIO_Init(D_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* Configure D_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = D_SPI_MOSI_PIN;
  GPIO_Init(D_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* Configure D_SPI_CS_PIN, D_LED_PIN and D_RST_PIN  */
  GPIO_InitStructure.GPIO_Pin = D_CS_PIN | D_RST_PIN  | D_LED_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(D_CS_GPIO_PORT, &GPIO_InitStructure);


  /* SD_SPI Config */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(D_SPI, &SPI_InitStructure);

  SPI_Cmd(D_SPI, ENABLE); /* SD_SPI enable */
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
  * @brief  Sends 1 Byte over SPI
  * @param  Byte that has to be send
  * @retval None
  */
uint8_t D_SPI_SendData(uint8_t data){

	/*!< Wait until the transmit buffer is empty */
	while(SPI_I2S_GetFlagStatus(D_SPI, SPI_I2S_FLAG_TXE) == RESET)
	{
	}
	SPI_I2S_SendData(D_SPI, data);

	/*!< Wait to receive a byte*/
	while(SPI_I2S_GetFlagStatus(D_SPI, SPI_I2S_FLAG_RXNE) == RESET)
	{
	}
	return SPI_I2S_ReceiveData(D_SPI);

}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
  * @brief  Receives 1 Byte that has been send over the SPI Bus
  * @param  None
  * @retval The received Byte
  */
uint8_t D_SPI_ReceiveData(){

	/*!< Wait until the transmit buffer is empty */
	while(SPI_I2S_GetFlagStatus(D_SPI, SPI_I2S_FLAG_TXE) == RESET)
	{
	}

	SPI_I2S_SendData(D_SPI, DUMMYBYTE);

	/*!< Wait to receive a byte*/
	while(SPI_I2S_GetFlagStatus(D_SPI, SPI_I2S_FLAG_RXNE) == RESET)
	{
	}

	return SPI_I2S_ReceiveData(D_SPI);
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
  * @brief  Changes the SPI Bus Speed
  * @param  the new SPI Bus Speed
  * 	@arg SPI_BaudRatePrescaler_2
  * 	@arg SPI_BaudRatePrescaler_4
  * 	@arg SPI_BaudRatePrescaler_8
  * 	@arg SPI_BaudRatePrescaler_16
  * 	@arg SPI_BaudRatePrescaler_32
  * 	@arg SPI_BaudRatePrescaler_64
  * 	@arg SPI_BaudRatePrescaler_128
  * 	@arg SPI_BaudRatePrescaler_256
  * @retval None
  */
void D_SPI_ChangeSPISpeed(uint16_t newBaudRatePrescaler){

	  SPI_InitTypeDef   SPI_InitStructure;

	  SPI_Cmd(D_SPI, DISABLE);

	  /* SD_SPI Config */
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = newBaudRatePrescaler;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_Init(D_SPI, &SPI_InitStructure);

	  SPI_Cmd(D_SPI, ENABLE); /* SD_SPI enable */

}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
  * @brief  Pulls CS High
  * @param  None
  * @retval None
  */
void D_SPI_CSHigh(){
	GPIO_SetBits(D_CS_GPIO_PORT, D_CS_PIN);
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
  * @brief  Pulls CS Low
  * @param  None
  * @retval None
  */
void D_SPI_CSLow(){
	GPIO_ResetBits(D_CS_GPIO_PORT, D_CS_PIN);
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
  * @brief  Pulls CS High
  * @param  None
  * @retval None
  */
void D_SPI_LEDHigh(){
	GPIO_SetBits(D_LED_GPIO_PORT, D_LED_PIN);
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
  * @brief  Pulls CS Low
  * @param  None
  * @retval None
  */
void D_SPI_LEDLow(){
	GPIO_ResetBits(D_LED_GPIO_PORT, D_LED_PIN);
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
  * @brief  Pulls CS High
  * @param  None
  * @retval None
  */
void D_SPI_RSTHigh(){
	GPIO_SetBits(D_RST_GPIO_PORT, D_RST_PIN);
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
  * @brief  Pulls CS Low
  * @param  None
  * @retval None
  */
void D_SPI_RSTLow(){
	GPIO_ResetBits(D_RST_GPIO_PORT, D_RST_PIN);
}
/*----------------------------------------------------------------------------*/
