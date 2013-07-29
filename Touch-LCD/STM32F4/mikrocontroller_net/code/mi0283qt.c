/*
 * mi0283qt.c
 *
 *  Created on: May 10, 2012
 *      Author: Phantomix
 */
#include "mi0283qt.h"

SPI_TypeDef* display_SPIx;

//Character Set
/*
const unsigned char _0[8] = {0x1C,0x22,0x26,0x2A,0x32,0x22,0x1C,0x00};
const unsigned char _1[8] = {0x08,0x18,0x08,0x08,0x08,0x08,0x1C,0x00};
const unsigned char _2[8] = {0x1C,0x22,0x02,0x04,0x08,0x10,0x3E,0x00};
const unsigned char _3[8] = {0x3E,0x04,0x08,0x04,0x02,0x22,0x1C,0x00};
const unsigned char _4[8] = {0x04,0x0C,0x14,0x24,0x3E,0x04,0x04,0x00};
const unsigned char _5[8] = {0x3E,0x20,0x3C,0x02,0x02,0x22,0x1C,0x00};
const unsigned char _6[8] = {0x0C,0x10,0x20,0x3C,0x22,0x22,0x1C,0x00};
const unsigned char _7[8] = {0x3E,0x22,0x02,0x04,0x08,0x08,0x08,0x00};
const unsigned char _8[8] = {0x1C,0x22,0x22,0x1C,0x22,0x22,0x1C,0x00};
const unsigned char _9[8] = {0x1C,0x22,0x22,0x1E,0x02,0x04,0x18,0x00};
const unsigned char _A[8] = {0x1C,0x22,0x22,0x22,0x3E,0x22,0x22,0x00};
const unsigned char _B[8] = {0x3C,0x22,0x22,0x3C,0x22,0x22,0x3C,0x00};
const unsigned char _C[8] = {0x1C,0x22,0x20,0x20,0x20,0x22,0x1C,0x00};
const unsigned char _D[8] = {0x38,0x24,0x22,0x22,0x22,0x24,0x38,0x00};
const unsigned char _E[8] = {0x3E,0x20,0x20,0x3C,0x20,0x20,0x3E,0x00};
const unsigned char _F[8] = {0x3E,0x20,0x20,0x3C,0x20,0x20,0x20,0x00};
const unsigned char _G[8] = {0x1C,0x22,0x20,0x2E,0x22,0x22,0x1E,0x00};
const unsigned char _H[8] = {0x22,0x22,0x22,0x3E,0x22,0x22,0x22,0x00};
const unsigned char _I[8] = {0x1C,0x08,0x08,0x08,0x08,0x08,0x1C,0x00};
const unsigned char _J[8] = {0x0E,0x04,0x04,0x04,0x04,0x24,0x18,0x00};
const unsigned char _K[8] = {0x22,0x24,0x28,0x30,0x28,0x24,0x22,0x00};
const unsigned char _L[8] = {0x20,0x20,0x20,0x20,0x20,0x20,0x3E,0x00};
const unsigned char _M[8] = {0x22,0x36,0x2A,0x2A,0x22,0x22,0x22,0x00};
const unsigned char _N[8] = {0x22,0x22,0x32,0x2A,0x26,0x22,0x22,0x00};
const unsigned char _O[8] = {0x1C,0x22,0x22,0x22,0x22,0x22,0x1C,0x00};
const unsigned char _P[8] = {0x3C,0x22,0x22,0x3C,0x20,0x20,0x20,0x00};
const unsigned char _Q[8] = {0x1C,0x22,0x22,0x22,0x2A,0x24,0x1A,0x00};
const unsigned char _R[8] = {0x3C,0x22,0x22,0x3C,0x28,0x24,0x22,0x00};
const unsigned char _S[8] = {0x1E,0x20,0x20,0x1C,0x02,0x02,0x3C,0x00};
const unsigned char _T[8] = {0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x00};
const unsigned char _U[8] = {0x22,0x22,0x22,0x22,0x22,0x22,0x1C,0x00};
const unsigned char _V[8] = {0x22,0x22,0x22,0x22,0x22,0x14,0x08,0x00};
const unsigned char _W[8] = {0x22,0x22,0x22,0x2A,0x2A,0x2A,0x14,0x00};
const unsigned char _X[8] = {0x22,0x22,0x14,0x08,0x14,0x22,0x22,0x00};
const unsigned char _Y[8] = {0x22,0x22,0x22,0x14,0x08,0x08,0x08,0x00};
const unsigned char _Z[8] = {0x3E,0x02,0x04,0x08,0x10,0x20,0x3E,0x00};
const unsigned char _none[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
*/

inline void delay_ms(uint16_t millis)
{
	millisec_countdown = millis;
	while(millisec_countdown);
}

inline void display_reset(uint8_t reset)
{
	if(reset)
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	else
		GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

inline void display_led(uint8_t led)
{
	if(led)
		GPIO_SetBits(GPIOD, GPIO_Pin_8);
	else
		GPIO_ResetBits(GPIOD, GPIO_Pin_8);
}
inline void touch_cs(uint8_t cs)
{
	if(cs)
		GPIO_ResetBits(GPIOD, GPIO_Pin_9);
	else
		GPIO_SetBits(GPIOD, GPIO_Pin_9);
}

inline void display_cs(uint8_t cs)
{
	if(cs)
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	else
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

void spi_wait()
{
	while(SPI_I2S_GetFlagStatus(display_SPIx, SPI_I2S_FLAG_BSY));
}
void spi_write(uint8_t c)
{
	spi_wait();
	SPI_I2S_SendData(display_SPIx, c);
}
uint8_t spi_receive()
{
	while(!SPI_I2S_GetFlagStatus(display_SPIx, SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(display_SPIx);
}
/*
RST     pb11
LED     pd8
CS_t    pd9
CS_dis  pb12
SCL     pb13
miso    pb14
mosi    pb15
 *
 */
void init_mi0283qt(SPI_TypeDef* SPIx)
{
	display_SPIx = SPIx;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	display_reset(1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	display_led(1);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = /*GPIO_Pin_12 |*/ GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//GPIO_PinAFConfig(GPIOB, 12, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, 13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, 14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, 15, GPIO_AF_SPI2);

	SPI_InitTypeDef SPI_InitStructure;
	SPI_DeInit(SPIx);
	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	//SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	//168 / 8 = 21 MHz
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//168 / 256 = 656 kHz
	//SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPIx, &SPI_InitStructure);

	SPI_Cmd(SPIx, ENABLE);

	lcd_reset();
}



//Multi-Inno MI0283QT-2 (HX8347D) - http://www.watterott.com


//#define LCD_MIRROR         //define to mirror the display
#define LCD_WIDTH            (320)
#define LCD_HEIGHT           (240)


#define LCD_ID               (0)
#define LCD_DATA             ((0x72)|(LCD_ID<<2))
#define LCD_REGISTER         ((0x70)|(LCD_ID<<2))

#define LCD_RST_DISABLE()    display_reset(0) //RST high
#define LCD_RST_ENABLE()     display_reset(1) //RST low
#define LCD_CS_DISABLE()     {__NOP();__NOP();__NOP();__NOP();__NOP(); display_cs(0);} //((void)0)	//CS high
#define LCD_CS_ENABLE()      {display_cs(1);__NOP();__NOP();__NOP();__NOP();} //((void)0)	//CS low

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

void lcd_draw(unsigned int color)
{
  spi_write(color>>8);
  spi_write(color);

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

void lcd_drawstop(void)
{
  spi_wait();
  LCD_CS_DISABLE();

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

void lcd_testscreen(void)
{
	uint16_t x;
	uint8_t y;
	uint8_t r,g,b;
	lcd_area(0, 0, (LCD_WIDTH-1), (LCD_HEIGHT-1));
	lcd_drawstart();
	for(y = 0; y < LCD_HEIGHT; y++)
	{
		for(x = 0; x < LCD_WIDTH; x++)
		{
			r = x + y;
			g = x - y;
			b = y - x;
			lcd_draw(RGB(r,g,b));
		}
	}
	lcd_drawstop();
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

void lcd_cursor(unsigned int x, unsigned int y)
{
  lcd_area(x, y, x, y);

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
