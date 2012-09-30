/*
 * mi0283qt.h
 *
 *  Created on: May 10, 2012
 *      Author: Phantomix
 */

#ifndef MI0283QT_H_
#define MI0283QT_H_
#include "stm32f4xx.h"
#include "config.h"

void init_mi0283qt(SPI_TypeDef* SPIx);
void lcd_draw(unsigned int color);
void lcd_drawstart(void);
void lcd_drawstop(void);
void lcd_area(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
void lcd_testscreen(void);
void lcd_clear(unsigned int color);
void lcd_cursor(unsigned int x, unsigned int y);
void lcd_data(unsigned int c);
void lcd_reset(void);

#define RGB(r,g,b)           (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue

#endif /* MI0283QT_H_ */
