/*
 * SD_HaLayer.h
 *
 *  Created on: 23.09.2010
 *      Author: Sebastian
 */

#ifndef D_HALAYER_H_
#define D_HALAYER_H_

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DUMMYBYTE 						0xFF
/* SPI Interface Init Defines */
#define D_SPI                           SPI2
#define D_SPI_CLK                       RCC_APB1Periph_SPI2
#define D_SPI_SCK_PIN                   GPIO_Pin_13                 /* PB.13 */
#define D_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOB */
#define D_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOB
#define D_SPI_MISO_PIN                  GPIO_Pin_14                 /* PB.14 */
#define D_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
#define D_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOB
#define D_SPI_MOSI_PIN                  GPIO_Pin_15                 /* PB.15 */
#define D_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOB */
#define D_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOB

/* Generel Pin Defines */
#define D_CS_PIN                        GPIO_Pin_12                 /* PA.12 */
#define D_CS_GPIO_PORT                  GPIOA                       /* GPIOA */
#define D_CS_GPIO_CLK                   RCC_APB2Periph_GPIOA

#define D_LED_PIN                       GPIO_Pin_11                 /* PA.11 */
#define D_LED_GPIO_PORT                 GPIOA                       /* GPIOA */
#define D_LED_GPIO_CLK                  RCC_APB2Periph_GPIOA

#define D_RST_PIN                       GPIO_Pin_10                 /* PA.10 */
#define D_RST_GPIO_PORT                 GPIOA                       /* GPIOA */
#define D_RST_GPIO_CLK                  RCC_APB2Periph_GPIOA


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void D_SPI_Init(void);
uint8_t D_SPI_SendData(uint8_t data);
uint8_t D_SPI_ReceiveData(void);
void D_SPI_ChangeSPISpeed(uint16_t newBaudRatePrescaler);
void D_SPI_CSHigh(void);
void D_SPI_CSLow(void);
void D_SPI_LEDHigh(void);
void D_SPI_LEDLow(void);
void D_SPI_RSTHigh(void);
void D_SPI_RSTLow(void);


#endif /* SD_HALAYER_H_ */
