// Quelle: http://www.watterott.net/forum/topic/243#1352


void delay_ms(uint32_t delayMs);
void lcd_reset(void);

#define LED_PIN (1 << 6)
#define LCD_CS_PIN (1 << 7)
#define LCD_RESET_PIN (1 << 8)

#define LCD_RST_DISABLE()    GPIOB->BSRRL = LCD_RESET_PIN
#define LCD_RST_ENABLE()     GPIOB->BSRRH = LCD_RESET_PIN
#define LCD_CS_DISABLE()     GPIOB->BSRRL = LCD_CS_PIN
#define LCD_CS_ENABLE()      GPIOB->BSRRH = LCD_CS_PIN
#define LED_ENABLE()         GPIOB->BSRRL = LED_PIN
#define LED_DISABLE()        GPIOB->BSRRH = LED_PIN

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

#define spi_wait() /* do nothing */

static inline void spi_write(uint8_t byte)
{
   SPI1->DR = byte;

   // Loop while DR register in not empty
   while (! (SPI1->SR & SPI_SR_TXE));

   // Loop while Receive Buffer is not empty
   while (! (SPI1->SR & SPI_SR_RXNE));

   // Receive byte
   volatile uint16_t tmp = SPI1->DR;
}
