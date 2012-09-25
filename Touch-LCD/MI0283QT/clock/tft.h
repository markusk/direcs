/*
 * tft.h
 *
 *  Created on: Dec 17, 2010
 *      Author: Sebastian M.
 */

#ifndef TFT_H_
#define TFT_H_

#include <stdio.h>
#include <math.h>
#include "stm32f10x.h"
#include "D_HaLayer.h"
#include "main.h"
#include "font_8x8.h"
#include "font_8x12.h"


/* Private define  ---------------------------------------------------------------------------------*/
//#define LCD_MIRROR         //define to mirror the display
#define LCD_WIDTH            (320)
#define LCD_HEIGHT           (240)
#define RGB(r,g,b)           (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue

#define LCD_ID               (0)
#define LCD_DATA             ((0x72)|(LCD_ID<<2))
#define LCD_REGISTER         ((0x70)|(LCD_ID<<2))


#define COLOR_WHITE  RGB(255,255,255)
#define COLOR_BLACK  RGB(  0,  0,  0)
#define COLOR_RED    RGB(255,  0,  0)
#define COLOR_GREEN  RGB(  0,255,  0)
#define COLOR_BLUE   RGB(  0,  0,255)
#define COLOR_YELLOW RGB(255,255,  0)

/* Private function prototypes ---------------------------------------------------------------------*/
void lcd_drawChar(unsigned int x0, unsigned int y0, unsigned char c, unsigned char typ,unsigned int color, unsigned int bg_color);
void lcd_drawString(unsigned int x0, unsigned int y0, char *str, unsigned char typ, unsigned int color, unsigned int bg_color);
void lcd_drawInt(unsigned int x0, unsigned int y0, int i, unsigned char typ,unsigned int color, unsigned int bg_color);
void lcd_drawHex(unsigned int x0, unsigned int y0, int i, unsigned char typ,unsigned int color, unsigned int bg_color);

void lcd_fillRect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color);
void lcd_drawRect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color);

void lcd_drawCircle(unsigned int x0, unsigned int y0, unsigned int radius, unsigned int color);
void lcd_fillCircle(unsigned int x0, unsigned int y0, unsigned int radius, unsigned int color);

void lcd_drawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color);
void lcd_degreeLine(unsigned int x0, unsigned int y0, int angle, unsigned int length, unsigned int color);
void lcd_verticalLine(unsigned int x0, unsigned int y0, unsigned int length, unsigned int color);
void lcd_horizontalLine(unsigned int x0, unsigned int y0, unsigned int length, unsigned int color);

void lcd_clear(unsigned int color);
void lcd_setPixel(unsigned int x0, unsigned int y0, unsigned int color);

void lcd_draw(unsigned int color);
void lcd_drawstop(void);
void lcd_drawstart(void);
void lcd_area(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
void lcd_cursor(unsigned int x, unsigned int y);

void lcd_cmd(unsigned int reg, unsigned int param);
void lcd_data(unsigned int c);
void lcd_reset(void);

#endif /* TFT_H_ */
