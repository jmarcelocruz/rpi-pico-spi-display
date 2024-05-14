CFLAGS = -mcpu=cortex-m0plus -mthumb -ffreestanding -Iexternal/hardware-regs/include -Iinclude ${EXTRA_CFLAGS}
LDFLAGS = -mcpu=cortex-m0plus -mthumb -nostdlib -Texternal/startup/rpi-pico.ld ${EXTRA_LDFLAGS}

SOURCES = main.c environment.c display/display.c spi/spi.c
OBJS = $(patsubst %c,%o,${SOURCES})

firmware.elf: firmware-no-crc.elf
	rpi-pico-elf-add-crc $^ $@

firmware-no-crc.elf: ${OBJS} external/startup/startup.o
	${CC} $^ ${LDFLAGS} -o $@

${OBJS}: %.o: %.c
	${CC} -c $^ ${CFLAGS} -o $@

external/startup/startup.o:
	${MAKE} -C external/startup startup.o

.PHONY: clean
clean:
	rm -f ${OBJS} firmware-no-crc.elf firmware.elf
