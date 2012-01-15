EXECUTABLE=main.elf
BIN_IMAGE=main.bin

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-g -O2 -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4	
CFLAGS+=-ffreestanding -nostdlib

# to run from FLASH
CFLAGS+=-Wl,-T,stm32_flash.ld

# Default C stuff
CFLAGS+=-I/usr/local/arm-none-eabi/include
LDFLAGS+=-L/usr/local/arm-none-eabi/lib

# stm32f4_discovery stuff
CFLAGS+=-I../../STM32F4xx_StdPeriph_Driver/inc
CFLAGS+=-I../../STM32F4xx_StdPeriph_Driver/inc/device_support
CFLAGS+=-I../../STM32F4xx_StdPeriph_Driver/inc/core_support 
LDFLAGS+=-L../../STM32F4xx_StdPeriph_Driver/build -lSTM32F4xx_StdPeriph_Driver

all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@

$(EXECUTABLE): main.c usart.c system_stm32f4xx.c startup_stm32f4xx.s stm32f4xx_it.c 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)

.PHONY: all clean
