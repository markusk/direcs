/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/usart.h>

void clock_setup(void)
{
	/* Enable GPIOD clock for LED & USARTs. */
	// port D (LEDs)
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);

	// port A (USART)
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);

	/* Enable clocks for USART2. */
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_USART2EN);
}

void usart_setup(void)
{
	// Setup USART2 parameters

	// 38400
	usart_set_baudrate(USART2, 38400);

	// 8N1
	usart_set_databits(USART2, 8);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_stopbits(USART2, USART_STOPBITS_1);

	// setup TX *and* RX mode
	usart_set_mode(USART2, USART_MODE_TX_RX);
	
	// no flow control
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	// Finally enable the USART.
	usart_enable(USART2);
}

void gpio_setup(void)
{
	// Setup GPIO pin GPIO12 on GPIO port D for LED.
	// PUPD_NONE = no pull up pull down resistor
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

	// Setup GPIO pins for USART2 transmit.
	// TX
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

	// RX
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);

	// RX as input mode! >> does not work, too!
	// gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO3);

	// Setup USART2 TX pin as alternate function.
	//
	// For PIN0 to 7 we have (p. 141, RM0090):
	// AF0 = system
	// AF1 = TIM1..2
	// AF2 = TIM3..5
	// AF3 = TIM8..11
	// AF4 = I2C1..3
	// AF5 = SPI1..2
	// AF6 = SPI3
	// AF7 = USART1..3
	// AF8 = USART4..6
	// AF9 = CAN1..2, TIM12..14
	// AF10 = OTG_FS, OTG_HS
	// AF11 = ETH
	// AF12 = FSMC, SDIO, OTG_HS
	// AF13 = DCMI
	// AF14 = n/a
	// AF15 = EVENTOUT
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);

	// RX
	gpio_set_af(GPIOA, GPIO_AF7, GPIO3); // < < < < <  enabling this stops TX-ing !! < < < < <
}

int main(void)
{
	int i, j = 0, c = 0;
	int value = 0;

	clock_setup();
	gpio_setup();
	usart_setup();

	// Blink the LED (PD12) on the board with every transmitted byte.
	while (1)
	{
		// Wait until we receive something on the serial port
		// Note that this is a BLOCKING read, so this will never be left, until we receive a byte!
		//
		value = usart_recv_blocking(USART2); // < < < < <  enabling this stops TX-ing !! < < < < <

		// original code:
		gpio_toggle(GPIOD, GPIO12);	// LED on/off

//		usart_send_blocking(USART2, c + '0'); // USART2: Send byte. 

//		c = (c == 9) ? 0 : c + 1;	// Increment c. 

		// Send byte
		usart_send_blocking(USART2, '@');
		 
		// Newline after line full.
		if ((j++ % 80) == 0)
		{ 
			usart_send_blocking(USART2, '\r');
			usart_send_blocking(USART2, '\n');
		}

		// Wait a bit
//		for (i = 0; i < 3000000; i++)
		for (i = 0; i < 200000; i++)
			__asm__("NOP");
	}

	return 0;
}
