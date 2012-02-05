/** \file printf.c
 * Simplified printf() and sprintf() implementation - prints formatted string to
 * USART (or whereever). Most common % specifiers are supported. It costs you about
 * 3k FLASH memory - without floating point support it uses only 1k ROM!
 * \author Freddie Chopin, Martin Thomas, Marten Petschke and many others
 * \date 20.9.2011, 2.11.2011
 */

/******************************************************************************
* chip: STM32F10x
* compiler: arm-none-eabi-gcc 4.6.0
*
* global functions:
* 	int printf_(const char *format, ...)
* 	int sprintf_(char *buffer, const char *format, ...)
*
* STM32 specific functions:
*         init_UART1(void)
*	void putc_UART1 (char);                // blocking put char, used by printf()
*
* local functions:
* 	int putc_strg(int character, printf_file_t *stream)
* 	int vfprintf_(printf_file_t *stream, const char *format, va_list arg)
* 	void long_itoa (long val, int radix, int len, vfprintf_stream *stream)
*
******************************************************************************/

/*
+=============================================================================+
| includes
+=============================================================================+
*/

#include <stdarg.h>     // (...) parameter handling
#include <stdlib.h>     //NULL pointer definition

#include "stm32f10x.h"	// only this headerfile is used
  #define assert_param(expr) ((void)0) /*dummy to make the stm32 header work*/

/*
+=============================================================================+
| options
+=============================================================================+
*/
//#define INCLUDE_FLOAT  // this enables float in printf() and costs you about 2kByte ROM
//#define USE_DEBUG      // this enables a Macro based on printf()
#define PLL_FREQUENCY			16000000ul	///< desired target frequency of the core
#define UART1_BAUDRATE  		115200
/*
+=============================================================================+
| global declarations
+=============================================================================+
*/

int printf_(const char *format, ...);
int sprintf_(char *buffer, const char *format, ...);

/*
+=============================================================================+
| local declarations
+=============================================================================+
*/
/// a simplified FILE struct
typedef struct printf_file_s
{
	void (*putc)(char c, char **buffer);	// put() function for writing one single char
	char *buffer;							// pointer to buffer for e.g. sprintf_()
} vfprintf_stream;

void init_UART1(void);                         // STM32 specific stuff

void putc_UART1 (char, char **buffer);         // blocking put char; used by printf_()
void putc_strg(char , char **buffer);          // the put() function for sprintf()

static int vfprintf_(vfprintf_stream *stream, const char *format, va_list arg); //generic print
void long_itoa (long, int, int, vfprintf_stream *stream); //heavily used by printf_()

/*
+=============================================================================+
| sample main()  file
+=============================================================================+
*/
int main(void){
	init_UART1();    
    printf_("\nSTM32_Start\n");
    debug ("debugging enabled");  
	int count;
    for (count = 0; count < 12; count++)	// test
	printf_("test unsigned negativ:      |%u|  \n", (unsigned int)(-count));

}

/*
+=============================================================================+
| STM32 register definition only here and in interrupt handler
+=============================================================================+
*/
void init_UART1(void){

	RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN | RCC_APB2ENR_USART1EN);  //Clock
	AFIO->MAPR &= ~(AFIO_MAPR_USART1_REMAP);		//UART1 at the original pins
	GPIOA->CRH  = 0x0090;  							//GPIO_CRH_BIT9 = Alternate function, 10MHz
    USART1->BRR  = (PLL_FREQUENCY+UART1_BAUDRATE/2)/UART1_BAUDRATE;
	             //0x008B;  Baud=FREQUENCY/(16*8,69)=115107;	// BRR simplified thanks to Uwe Hermann
    USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE);  // RX, TX enable
}

void putc_UART1 (char c, char **buffer)
{
	buffer = buffer;                               //unused
	if (c == '\n') {
		while ((USART1->SR & USART_SR_TXE) == 0);  //blocks until previous byte was sent
		USART1->DR ='\r';
	}
	while ((USART1->SR & USART_SR_TXE) == 0);  //blocks until previous byte was sent
	USART1->DR = c;
}

/*
+=============================================================================+
| end of controller specific stuff  - no further controller dependent stuff below
+=============================================================================+
*/

/*
+=============================================================================+
| global functions
+=============================================================================+
*/
int printf_(const char *format, ...)
{
	va_list arg;
	vfprintf_stream stream;

	stream.buffer=NULL;
	stream.putc=(&putc_UART1);

	va_start(arg, format);
	vfprintf_(&stream, format, arg);
	va_end(arg);

	return 0;
}

int sprintf_(char *buffer, const char *format, ...)
{
	va_list arg;
	vfprintf_stream stream;

	stream.buffer=buffer;	 //Pointer auf einen String in Speicherzelle abspeichern
	stream.putc=(&putc_strg);


	va_start(arg, format);
	vfprintf_(&stream, format, arg);
	va_end(arg);

//	buffer='\0'; //add end of string

	return 0;
}

/**
 * @def debug(format...)
 * @brief prints the timestamp, file name, line number, printf-formated @a format string and the
 * optional parameters to stdout
 *
 * The output looks like this:<br>
 * <pre>
 * 12345     filename.c[123]: format string
 * ^    ^    ^          ^
 * |    |    |          line number
 * |    |    +--------- file name
 * |    +-------------- tab character
 * +------------------- timestamp (ms since reset)
 * </pre>
 *
 * */
#ifdef USE_DEBUG
	#define debug(format,...) {\
		printf_("%ul\t%s[%i]: ", millisec, __FILE__, __LINE__); /* print file name and line number */\
		printf_(format, ## __VA_ARGS__);               /* print format string and args */\
		printf_("\n"); \
	}
#else
	#define debug(format,...) ((void)0)
#endif /* USE_DEBUG */



/*
+=============================================================================+
| local functions
+=============================================================================+
*/
// putc_strg() is the putc()function for sprintf_()
void putc_strg(char character,char **buffer)
{
	**buffer = (char)character;	// just add the character to buffer
	(*buffer)++;
	**buffer ='\0';             // append end of string

}

/*--------------------------------------------------------------------------------+
 * vfprintf_()
 * Prints a string to stream. Supports %s, %c, %d, %ld %ul %02d %i %x  %lud  and %%
 *     - partly supported: long long, float (%ll, %f, %F, %2.2f)
 *     - not supported: double float and exponent (%e %g %p %o \t)
 *--------------------------------------------------------------------------------+
*/
static int vfprintf_(vfprintf_stream *stream, const char* str,  va_list arp)
{
	int d, r, w, s, l;  //d=char, r = radix, w = width, s=zeros, l=long
	char *c;            // for the while loop only

#ifdef INCLUDE_FLOAT
	float f;
	long int m, w2;
#endif

	while ((d = *str++) != 0) {
		if (d != '%') {
			(*stream->putc)(d, &(stream->buffer));
			continue;
		}
		d = *str++;
		w = r = s = l = 0;
		if (d == '%') {
			(*stream->putc)(d, &(stream->buffer));
			d = *str++;
		}
		if (d == '0') {
			d = *str++; s = 1;  //
		}
		while ((d >= '0')&&(d <= '9')) {
			w += w * 10 + (d - '0');
			d = *str++;
		}
		if (s) w = -w;      //padd with zeros if negative

	#ifdef INCLUDE_FLOAT
		w2 = 0;
		if (d == '.')
			d = *str++;
		while ((d >= '0')&&(d <= '9')) {
			w2 += w2 * 10 + (d - '0');
			d = *str++;
		}
	#endif

		if (d == 's') {
			c = va_arg(arp, char*);
			while (*c)
				(*stream->putc)(*(c++), &(stream->buffer));
			continue;
		}
		if (d == 'c') {
			(*stream->putc)((char)va_arg(arp, int), &(stream->buffer));
			continue;
		}
		if (d == 'u') {     // %ul
			r = 10;
			d = *str++;
		}
		if (d == 'l') {     // long =32bit
			l = 1;
			if (r==0) r = -10;
			d = *str++;
			if (d == 'l') { // %ll long long =64bit
				l++; // = 2;
				d = *str++;
			}
			if (d == 'u') { // %lui
				r = 10;
				d = *str++;
			}
		}
//		if (!d) break;
		if (d == 'd' || d == 'i') {if (r==0) r = -10;}  //can be 16 or 32bit int
		else if (d == 'X' || d == 'x') r = 16; // 'x' added by mthomas in increase compatibility
		else if (d == 'b') r = 2;
		else str--;

	#ifdef INCLUDE_FLOAT
		if (d == 'f' || d == 'F') {
			f=va_arg(arp, double);
			if (f>0) {
				r=10;
				m=(int)f;
			}
			else {
				r=-10;
				f=-f;
				m=(int)(f);
			}
			long_itoa(m, r, w, stream);
			f=f-m; m=f*(10^w2); w2=-w2;
			long_itoa(m, r, w2, stream);
			l=-1; //do not continue with long
		}
	#endif

		if (!r) continue;  
		if ((l==0) | (l==1)) {
			if (r > 0)      //unsigned
				long_itoa((unsigned long)va_arg(arp, int), r, w, stream); //needed for 16bit int, no harm to 32bit int
			else            //signed
				long_itoa((long)va_arg(arp, int), r, w, stream);
		} else if (l==2){            //long long =64bit
				;//longlong_itoa((unsigned long long)va_arg(arp, int), r, w, stream);
		}
	}

	return 0;
}

void long_itoa (long val, int radix, int len, vfprintf_stream *stream)
{
	char c, sgn = 0, pad = ' ';
	char s[20];
	int  i = 0;


	if (radix < 0) {
		radix = -radix;
		if (val < 0) {
			val = -val;
			sgn = '-';
		}
	}
	if (len < 0) {
		len = -len;
		pad = '0';
	}
	if (len > 20) return;
	do {
		c = (char)((unsigned long)val % radix); //cast!
		if (c >= 10) c += 7; //ABCDEF
		c += '0';            //0123456789
		s[i++] = c;
		val = (unsigned long)val /radix; //cast!
	} while (val);
	if (sgn) s[i++] = sgn;
	while (i < len)
		s[i++] = pad;
	do
		(*stream->putc)(s[--i],&(stream->buffer));
	while (i);
}


/******************************************************************************
* END OF FILE
******************************************************************************/
