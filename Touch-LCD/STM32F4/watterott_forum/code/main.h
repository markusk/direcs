#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx_conf.h"
#include <stdlib.h> // lets check this if this wokrs or is needed !!


//Multi-Inno MI0283QT-2 (HX8347D) - http://www.watterott.com


//#define LCD_MIRROR         //define to mirror the display
#define LCD_WIDTH            (320)
#define LCD_HEIGHT           (240)
#define RGB(r,g,b)           (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue

#define LCD_ID               (0)
#define LCD_DATA             ((0x72)|(LCD_ID<<2))
#define LCD_REGISTER         ((0x70)|(LCD_ID<<2))


//#define LCD_RST_DISABLE()    RST high
//#define LCD_RST_ENABLE()     RST low
//#define LCD_CS_DISABLE()     CS high
//#define LCD_CS_ENABLE()      CS low

// - - - - - 
#define LED_PIN (1 << 6)
#define LCD_CS_PIN (1 << 7)
#define LCD_RESET_PIN (1 << 8)

#define LCD_RST_DISABLE()    GPIOB->BSRRL = LCD_RESET_PIN
#define LCD_RST_ENABLE()     GPIOB->BSRRH = LCD_RESET_PIN
#define LCD_CS_DISABLE()     GPIOB->BSRRL = LCD_CS_PIN
#define LCD_CS_ENABLE()      GPIOB->BSRRH = LCD_CS_PIN
#define LED_ENABLE()         GPIOB->BSRRL = LED_PIN
#define LED_DISABLE()        GPIOB->BSRRH = LED_PIN
// - - - - -
#define COLOR_WHITE  RGB(255,255,255)
#define COLOR_BLACK  RGB(  0,  0,  0)
#define COLOR_RED    RGB(255,  0,  0)
#define COLOR_GREEN  RGB(  0,255,  0)
#define COLOR_BLUE   RGB(  0,  0,255)
#define COLOR_YELLOW RGB(255,255,  0)

#endif
