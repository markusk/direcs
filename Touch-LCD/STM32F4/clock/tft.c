/*
 * tft.h
 *
 *  Created on: Dec 17, 2010
 *      Author: Sebastian M.
 */

#include "tft.h"

/*Draw Functions--------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/**
 * @brief Stellt ein Char dar
 *
 * @param	x0			die X Koordinate
 * @param	y0			die Y Koordinate
 * @param	c			der darzustellende Char
 * @param	color		die Farbe des Chars
 * @param	bg_color	die Hintergrundfarbe
 *
 */
void lcd_drawChar(unsigned int x0, unsigned int y0, unsigned char c, unsigned char typ, unsigned int color, unsigned int bg_color){

	unsigned int i, j, tmp, font_width, font_height;

	if(typ == FONT8X8){
		//FONT8X8
		font_width = FONT8X8_WIDTH;
		font_height = FONT8X8_HEIGHT;
	}
	else{
		//FONT8X12
		font_width = FONT8X12_WIDTH;
		font_height = FONT8X12_HEIGHT;
	}

	if(c < 0x20){
		//Error, der angegebene Char kann nicht dargestellt werden, da er kleiner als 0x20 ist
		return;
	}

	//Grˆﬂenabfrage, passt der Char noch auf den Bildschirm?
	if((x0 + font_width - 1) > LCD_WIDTH || (y0 + font_height - 1) > LCD_HEIGHT){
		//Error, der Char kann nicht mehr dargestellt werden
		return;
	}

	lcd_area(x0, y0, x0 + font_width - 1, y0 + font_height - 1);

	lcd_drawstart();
	for(i = 0 ; i < font_height; i++){
		if(typ == FONT8X8)
			tmp = font_8x8[(c - 0x20) * font_height + i];
		else
			tmp = font_8x12[(c - 0x20) * font_height + i];

		for(j = 8; j != 0; j--){
			if(tmp & (1<<j))
				lcd_draw(color);
			else
				lcd_draw(bg_color);
		}
	}
	lcd_drawstop();
}

/**
 * @brief Stellt einen String dar
 *
 * @param	x0			die X Koordinate
 * @param	y0			die Y Koordinate
 * @param	*str		der darzustellende String
 * @param	color		die Farbe des Strings
 * @param	bg_color	die Hintergrundfarbe
 *
 */
void lcd_drawString(unsigned int x0, unsigned int y0, char *str, unsigned char typ, unsigned int color, unsigned int bg_color){

	unsigned char i = 0;

	while(*str){
		lcd_drawChar(x0 + (i*8), y0, (unsigned char) *str, typ, color, bg_color);
		str++;
		i++;
	}
}

/**
 * @brief Stellt einen Signed Integer dar
 *
 * @param	x0			die X Koordinate
 * @param	y0			die Y Koordinate
 * @param	*str		der darzustellende Int
 * @param	color		die Farbe des Strings
 * @param	bg_color	die Hintergrundfarbe
 *
 */
void lcd_drawInt(unsigned int x0, unsigned int y0, int i, unsigned char typ,unsigned int color, unsigned int bg_color){

	char buffer[16];
	sprintf(buffer, "%i", i);
	lcd_drawString(x0, y0, buffer, typ, color, bg_color);
}

/**
 * @brief Stellt einen Unsigned Integer dar
 *
 * @param	x0			die X Koordinate
 * @param	y0			die Y Koordinate
 * @param	*str		der darzustellende Int
 * @param	color		die Farbe des Strings
 * @param	bg_color	die Hintergrundfarbe
 *
 */
void lcd_drawHex(unsigned int x0, unsigned int y0, int i, unsigned char typ, unsigned int color, unsigned int bg_color){

	char buffer[16];
	sprintf(buffer, "%x", i);
	lcd_drawString(x0, y0, buffer, typ, color, bg_color);
}



/**
 * @brief Zeichnet ein Rechteck und f¸llt es mit Farbe aus
 *
 * Das Rechteck wird von den Anfangskoordinaten,
 * einschlieﬂlich bis zu den Endkoordinaten gezeichnet
 *
 * @param	x0 die X Anfangskoordinate
 * @param	y0 die Y Anfangskoordinate
 * @param	x1 die X Endkoordinate
 * @param	y1 die Y Endkoordinate
 * @param	color die Farbe des Rechteckes
 */
void lcd_fillRect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color){

	unsigned int i;
	unsigned int tmp;

	if(x0 > x1){
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	else if(y0 > y1){
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	lcd_area(x0, y0, x1, y1);

	lcd_drawstart();
	for(i = (x1 - x0 + 1) * (y1 - y0 + 1); i != 0; i--)
		lcd_draw(color);
	lcd_drawstop();
}

/**
 * @brief Zeichnet ein Rechteck
 *
 * Das Rechteck wird von den Anfangskoordinaten,
 * einschlieﬂlich bis zu den Endkoordinaten gezeichnet
 *
 * @param	x0 die X Anfangskoordinate
 * @param	y0 die Y Anfangskoordinate
 * @param	x1 die X Endkoordinate
 * @param	y1 die Y Endkoordinate
 * @param	color die Farbe des Rechteckes
 */
void lcd_drawRect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color){

	unsigned int tmp;

	if(x0 > x1){
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	else if(y0 > y1){
		tmp = y0;
		y0 = y1;
		y1 = y0;
	}

	lcd_verticalLine(x0, y0, y1 - y0 + 1, color);
	lcd_verticalLine(x1, y0, y1 - y0 + 1, color);

	lcd_horizontalLine(x0, y0, x1 - x0 + 1, color);
	lcd_horizontalLine(x0, y1, x1 - x0 + 1, color);
}

/**
 * @brief Zeichnet ein Kreis und f¸llt diesen mit Farbe aus
 *
 * Ein Kreis wird gezeichnet. Die angegeben Koordianten
 * beziehen sich auf den Kreismittelpunkt. Der Kreis wird
 * mit der angegebenen Farbe ausgef¸llt
 *
 * @param	x0 		die X Koordinate des Mittelpunkts
 * @param	y0 		die Y Koordinate des Mittelpunkts
 * @param	radius 	der Radius des Kreises
 * @param	color 	die Farbe des Kreises
 */
void lcd_fillCircle(unsigned int x0, unsigned int y0, unsigned int radius, unsigned int color){
	int err, x, y;

	err = -radius;
	x   = radius;
	y   = 0;

	lcd_area(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);

	while(x >= y){

		lcd_drawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
		lcd_drawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
		lcd_drawLine(x0 - y, y0 + x, x0 + y, y0 + x, color);
		lcd_drawLine(x0 - y, y0 - x, x0 + y, y0 - x, color);

		err += y;
		y++;
		err += y;
		if(err >= 0)
		{
		  x--;
		  err -= x;
		  err -= x;
		}
	}
}

/**
 * @brief Zeichnet ein Kreis
 *
 * Ein Kreis wird gezeichnet. Die angegeben Koordianten
 * beziehen sich auf den Kreismittelpunkt
 *
 * @param	x0 		die X Koordinate des Mittelpunkts
 * @param	y0 		die Y Koordinate des Mittelpunkts
 * @param	radius 	der Radius des Kreises
 * @param	color 	die Farbe des Kreises
 */
void lcd_drawCircle(unsigned int x0, unsigned int y0, unsigned int radius, unsigned int color){
	int err, x, y;

	err = -radius;
	x   = radius;
	y   = 0;

	lcd_area(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);

	while(x >= y){

		lcd_setPixel(x0 + x, y0 + y, color);
		lcd_setPixel(x0 - x, y0 + y, color);
		lcd_setPixel(x0 + x, y0 - y, color);
		lcd_setPixel(x0 - x, y0 - y, color);
		lcd_setPixel(x0 + y, y0 + x, color);
		lcd_setPixel(x0 - y, y0 + x, color);
		lcd_setPixel(x0 + y, y0 - x, color);
		lcd_setPixel(x0 - y, y0 - x, color);

		err += y;
		y++;
		err += y;
		if(err >= 0)
		{
		  x--;
		  err -= x;
		  err -= x;
		}
	}
}

/**
 * @brief Zeichnet eine Linie
 *
 * Die Linie wird von den Anfangskoordinaten,
 * einschlieﬂlich bis zu den Endkoordinaten gezeichnet
 *
 * @param	x0 die X Anfangskoordinate
 * @param	y0 die Y Anfangskoordinate
 * @param	x1 die X Endkoordinate
 * @param	y1 die Y Endkoordinate
 * @param	color die Farbe der Linie
 */
void lcd_drawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color){

	int dx, dy, dx2, dy2, err, stepx, stepy;

	if((x0 == x1) || (y0 == y1)){
		//horizontal or vertical line
		lcd_fillRect(x0, y0, x1, y1, color);
	}
	else{
	    //calculate direction
	    dx = x1 - x0;
	    dy = y1 - y0;
	    if(dx < 0) { dx = -dx; stepx = -1; } else { stepx = +1; }
	    if(dy < 0) { dy = -dy; stepy = -1; } else { stepy = +1; }
	    dx2 = dx << 1;
	    dy2 = dy << 1;
	    //draw line
	    //lcd_area(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
	    lcd_setPixel(x0, y0, color);
	    if(dx > dy)
	    {
	      err = dy2 - dx;
	      while(x0 != x1)
	      {
	        if(err >= 0)
	        {
	          y0  += stepy;
	          err -= dx2;
	        }
	        x0  += stepx;
	        err += dy2;
	        lcd_setPixel(x0, y0, color);
	      }
	    }
	    else
	    {
	      err = dx2 - dy;
	      while(y0 != y1)
	      {
	        if(err >= 0)
	        {
	          x0  += stepx;
	          err -= dy2;
	        }
	        y0  += stepy;
	        err += dx2;
	        lcd_setPixel(x0, y0, color);
	      }
	    }
	}
}

/**
 * @brief Zeichnet eine Linie
 *
 * Die Linie wird von den Anfangskoordinaten,
 * einschlieﬂlich bis zu den Endkoordinaten gezeichnet.
 * Der angegebene Radius bezieht sich auf den Winkel im 1 Quadranten.
 * 		|\
 * x ->	| \
 * 		|a \  <--- Winkel
 * -----|-----
 * ^	|
 * y	|
 *
 * @param	x0 			die X Anfangskoordinate
 * @param	y0 			die Y Anfangskoordinate
 * @param	angle		der Winkel
 * @param	length 		die L‰nge der Linie
 * @param	color 		die Farbe der Linie
 */
void lcd_degreeLine(unsigned int x0, unsigned int y0, int angle, unsigned int length, unsigned int color){

	lcd_drawLine(x0, y0, (unsigned int) (x0 + length* cos(angle * M_PI / 180)) , (unsigned int) (y0 - length* sin(angle * M_PI / 180)), color);
}



/**
 * @brief Zeichnet eine vertikale Linie (|)
 *
 * Die Linie wird von den Anfangskoordinaten
 * mit der angegebenen L‰nge gezeichnet gezeichnet
 *
 * @param	x0 die X Anfangskoordinate
 * @param	y0 die Y Anfangskoordinate
 * @param	lenght die L‰nge
 * @param	color die Farbe des Rechteckes
 */
void lcd_verticalLine(unsigned int x0, unsigned int y0, unsigned int length, unsigned int color){

	unsigned int i;

	lcd_area(x0, y0, x0, y0 + length - 1);

	lcd_drawstart();
	for(i = length - 1; i != 0; i--)
		lcd_draw(color);
	lcd_drawstop();
}

/**
 * @brief Zeichnet eine horizontale Linie (-)
 *
 * Die Linie wird von den Anfangskoordinaten
 * mit der angegebenen L‰nge gezeichnet gezeichnet
 *
 * @param	x0 die X Anfangskoordinate
 * @param	y0 die Y Anfangskoordinate
 * @param	lenght die L‰nge
 * @param	color die Farbe des Rechteckes
 */
void lcd_horizontalLine(unsigned int x0, unsigned int y0, unsigned int length, unsigned int color){

	unsigned int i;

	lcd_area(x0, y0, x0 + length - 1, y0);

	lcd_drawstart();
	for(i = length - 1; i != 0; i--)
		lcd_draw(color);
	lcd_drawstop();
}


/**
 * @brief Lehrt den Bildschirminhalt mit der angegeben Fabe
 *
 * @param	color die Farbe
 */
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

}

/**
 * @brief Setzt einen Pixel mit der angegeben Farbe
 *
 * @param	x0 die X Koordinate
 * @param	y0 die Y Koordinate
 * @param	color die Farbe des Pixels
 */
void lcd_setPixel(unsigned int x0, unsigned int y0, unsigned int color){

	lcd_cursor(x0, y0);

	lcd_drawstart();
	lcd_draw(color);
	lcd_drawstop();

}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*Basic Display Functions-----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void lcd_draw(unsigned int color)
{
	D_SPI_SendData(color>>8);
	D_SPI_SendData(color);

}


void lcd_drawstop(void)
{
  D_SPI_CSHigh();

}


void lcd_drawstart(void)
{
  D_SPI_CSLow();
  D_SPI_SendData(LCD_REGISTER);
  D_SPI_SendData(0x22);

  D_SPI_CSHigh();
  D_SPI_CSLow();
  D_SPI_SendData(LCD_DATA);

  return;
}


void lcd_area(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
	/* Alle Addressen sind 16 Bit Variablen,
	 * zuerst wird das High Byte und danach das
	 * Low Byte ¸bertagen */
	lcd_cmd(0x03, (x0>>0)); //set x0 Column address start register
	lcd_cmd(0x02, (x0>>8)); //set x0
	lcd_cmd(0x05, (x1>>0)); //set x1 Column address end register
	lcd_cmd(0x04, (x1>>8)); //set x1
	lcd_cmd(0x07, (y0>>0)); //set y0 Row address start register
	lcd_cmd(0x06, (y0>>8)); //set y0
	lcd_cmd(0x09, (y1>>0)); //set y1 Row address end register
	lcd_cmd(0x08, (y1>>8)); //set y1

  return;
}


void lcd_cursor(unsigned int x, unsigned int y)
{
  lcd_area(x, y, x, y);

  return;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*Basic Communication Funtions-----------------------------------------------*/
/*----------------------------------------------------------------------------*/
void lcd_cmd(unsigned int reg, unsigned int param)
{
  D_SPI_CSLow();
  D_SPI_SendData(LCD_REGISTER);
  D_SPI_SendData(reg);
  D_SPI_CSHigh();

  D_SPI_CSLow();
  D_SPI_SendData(LCD_DATA);
  D_SPI_SendData(param);
  D_SPI_CSHigh();

  return;
}


void lcd_data(unsigned int c)
{
  D_SPI_CSLow();
  D_SPI_SendData(LCD_DATA);
  D_SPI_SendData(c>>8);
  D_SPI_SendData(c);
  D_SPI_CSHigh();

  return;
}


void lcd_reset(void)
{
  //reset
  D_SPI_CSHigh();
  D_SPI_RSTLow();
  Delay(50);
  D_SPI_RSTHigh();
  Delay(50);

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
  Delay(5);
  lcd_cmd(0x1F, 0x0080);
  Delay(5);
  lcd_cmd(0x1F, 0x0090);
  Delay(5);
  lcd_cmd(0x1F, 0x00D0);
  Delay(5);

  //color selection
  lcd_cmd(0x17, 0x0005); //0x0005=65k, 0x0006=262k

  //panel characteristic
  lcd_cmd(0x36, 0x0000);

  //display on
  lcd_cmd(0x28, 0x0038);
  Delay(40);
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

