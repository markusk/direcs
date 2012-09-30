/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
 *   www.direcs.de                                                       *
 *                                                                       *
 *   This file is part of direcs.                                        *
 *                                                                       *
 *   direcs is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   direcs is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                       *
 *************************************************************************/

#include "main.h"
#include "stm32f4xx_conf.h"
#include <stdlib.h>



// Private variables ---------------------------------------------------------

// stores the serial received command and the string which will be sent as an answer
char stringbuffer[64];
int i;


// the prototypes ------------------------------------------------------------
int main(void);
void lcdInit();
// - -
void lcd_clear(unsigned int color);
void lcd_draw(unsigned int color);
void lcd_drawstop(void);
void lcd_drawstart(void);
void lcd_area(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
void lcd_cursor(unsigned int x, unsigned int y);
void lcd_cmd(unsigned int reg, unsigned int param);
void lcd_data(unsigned int c);
void lcd_reset(void);
// - -
void spi_wait(void);
void spi_write(uint8_t byte);
void delay(__IO uint32_t nCount);
void delay_ms(unsigned int ms);


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
}


void lcd_clear(unsigned int color)
{
  unsigned int i;

  lcd_area(0, 0, (LCD_WIDTH-1), (LCD_HEIGHT-1));

  lcd_drawstart();
  for(i=(LCD_WIDTH*LCD_HEIGHT/8); i!=0; i--)
  {
    lcd_draw(color); //1
    lcd_draw(color); //2
    lcd_draw(color); //3
    lcd_draw(color); //4
    lcd_draw(color); //5
    lcd_draw(color); //6
    lcd_draw(color); //7
    lcd_draw(color); //8
  }
  lcd_drawstop();

  return;
}


void lcd_draw(unsigned int color)
{
  spi_write(color>>8);
  spi_write(color);

  return;
}


void lcd_drawstop(void)
{
  spi_wait();
  LCD_CS_DISABLE();

  return;
}


void lcd_drawstart(void)
{
  LCD_CS_ENABLE();
  spi_write(LCD_REGISTER);
  spi_write(0x22);
  spi_wait();
  LCD_CS_DISABLE();

  LCD_CS_ENABLE();
  spi_write(LCD_DATA);

  return;
}


void lcd_area(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
  lcd_cmd(0x03, (x0>>0)); //set x0
  lcd_cmd(0x02, (x0>>8)); //set x0
  lcd_cmd(0x05, (x1>>0)); //set x1
  lcd_cmd(0x04, (x1>>8)); //set x1
  lcd_cmd(0x07, (y0>>0)); //set y0
  lcd_cmd(0x06, (y0>>8)); //set y0
  lcd_cmd(0x09, (y1>>0)); //set y1
  lcd_cmd(0x08, (y1>>8)); //set y1

  return;
}


void lcd_cursor(unsigned int x, unsigned int y)
{
  lcd_area(x, y, x, y);

  return;
}


void lcd_cmd(unsigned int reg, unsigned int param)
{
  LCD_CS_ENABLE();
  spi_write(LCD_REGISTER);
  spi_write(reg);
  spi_wait();
  LCD_CS_DISABLE();

  LCD_CS_ENABLE();
  spi_write(LCD_DATA);
  spi_write(param);
  spi_wait();
  LCD_CS_DISABLE();

  return;
}


void lcd_data(unsigned int c)
{
  LCD_CS_ENABLE();
  spi_write(LCD_DATA);
  spi_write(c>>8);
  spi_write(c);
  spi_wait();
  LCD_CS_DISABLE();

  return;
}


void lcd_reset(void)
{
  //reset
  LCD_CS_DISABLE();
  LCD_RST_ENABLE();
  delay_ms(50);
  LCD_RST_DISABLE();
  delay_ms(50);

  //driving ability
  lcd_cmd(0xEA, 0x0000);
  lcd_cmd(0xEB, 0x0020);
  lcd_cmd(0xEC, 0x000C);
  lcd_cmd(0xED, 0x00C4);
  lcd_cmd(0xE8, 0x0040);
  lcd_cmd(0xE9, 0x0038);
  lcd_cmd(0xF1, 0x0001);
  lcd_cmd(0xF2, 0x0010);
  lcd_cmd(0x27, 0x00A3);

  //power voltage
  lcd_cmd(0x1B, 0x001B);
  lcd_cmd(0x1A, 0x0001);
  lcd_cmd(0x24, 0x002F);
  lcd_cmd(0x25, 0x0057);

  //VCOM offset
  lcd_cmd(0x23, 0x008D); //for flicker adjust

  //power on
  lcd_cmd(0x18, 0x0036);
  lcd_cmd(0x19, 0x0001); //start osc
  lcd_cmd(0x01, 0x0000); //wakeup
  lcd_cmd(0x1F, 0x0088);
  delay_ms(5);
  lcd_cmd(0x1F, 0x0080);
  delay_ms(5);
  lcd_cmd(0x1F, 0x0090);
  delay_ms(5);
  lcd_cmd(0x1F, 0x00D0);
  delay_ms(5);

  //color selection
  lcd_cmd(0x17, 0x0005); //0x0005=65k, 0x0006=262k

  //panel characteristic
  lcd_cmd(0x36, 0x0000);

  //display on
  lcd_cmd(0x28, 0x0038);
  delay_ms(40);
  lcd_cmd(0x28, 0x003C);

  //display options
#ifdef LCD_MIRROR
  lcd_cmd(0x16, 0x0068); //MY=0 MX=1 MV=1 ML=0 BGR=1
#else
  lcd_cmd(0x16, 0x00A8); //MY=1 MX=0 MV=1 ML=0 BGR=1
#endif

  lcd_area(0, 0, (LCD_WIDTH-1), (LCD_HEIGHT-1));

  return;
}


void lcdInit()
{
   // Setup SPI
#ifdef USE_STDPERIPH_DRV

   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

   GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

   // LCD_CS_PIN / LCD_RESET_PIN / LED_PIN
   GPIO_InitTypeDef lcdGpio;
   lcdGpio.GPIO_Mode = GPIO_Mode_OUT;
   lcdGpio.GPIO_OType = GPIO_OType_PP;
   lcdGpio.GPIO_Pin = LCD_RESET_PIN | LED_PIN | LCD_CS_PIN;
   lcdGpio.GPIO_PuPd = GPIO_PuPd_DOWN;
   lcdGpio.GPIO_Speed = GPIO_Speed_100MHz;

   GPIO_Init(GPIOB, &lcdGpio);

   // SCK (PB3) / MISO (PB4) / MOSI (PB5)
   GPIO_InitTypeDef spiGpio;
   spiGpio.GPIO_Mode = GPIO_Mode_AF;
   spiGpio.GPIO_OType = GPIO_OType_PP;
   spiGpio.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
   spiGpio.GPIO_PuPd = GPIO_PuPd_DOWN;
   spiGpio.GPIO_Speed = GPIO_Speed_100MHz;

   GPIO_Init(GPIOB, &spiGpio);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

   SPI_InitTypeDef spiInit;
   spiInit.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   spiInit.SPI_DataSize = SPI_DataSize_8b;
   spiInit.SPI_CPOL = SPI_CPOL_High;
   spiInit.SPI_CPHA = SPI_CPHA_2Edge;
   spiInit.SPI_NSS = SPI_NSS_Soft;
   spiInit.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // 21 Mhz Takt
   spiInit.SPI_FirstBit = SPI_FirstBit_MSB;
   spiInit.SPI_CRCPolynomial = 7;
   spiInit.SPI_Mode = SPI_Mode_Master;

   SPI_Init(SPI1, &spiInit);

   SPI_Cmd(SPI1, ENABLE);
#else

#endif

   lcd_reset();

   LED_ENABLE();

   int delay = 500;

   for (;;)
   {
      delay_ms(delay);
      lcd_clear(COLOR_BLACK);
      delay_ms(delay);
      lcd_clear(COLOR_RED);
      delay_ms(delay);
      lcd_clear(COLOR_GREEN);
      delay_ms(delay);
      lcd_clear(COLOR_BLUE);
      delay_ms(delay);
      lcd_clear(COLOR_YELLOW);
      delay_ms(delay);
      lcd_clear(COLOR_WHITE);
      delay_ms(delay);
      lcd_clear(COLOR_BLACK);
      delay_ms(3000);
   }
}


void spi_wait(void)
{
  while (SPI1->SR & SPI_SR_BSY);
}


void spi_write(uint8_t byte)
{
   SPI1->DR = byte;

   // Loop while DR register in not empty
   while (! (SPI1->SR & SPI_SR_TXE));
}


/**
	* @brief  delay Function.
	* @param  nCount:specifies the delay time length.
  * @retval None
	*/
void delay(__IO uint32_t nCount)
{
	while(nCount--)
	{
	}
}


void delay_ms(unsigned int ms)
{

  delay(16795 * ms);
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
	  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	// Infinite loop
	while (1)
	{
	}
}
#endif
