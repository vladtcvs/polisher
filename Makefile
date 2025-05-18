default: firmware.bin

clean:
	rm -f *.o

%.o : %.c
	avr-gcc $< -mmcu=atmega328p -Os -c -o $@

firmware.elf : cmd.o core.o hw.o main.o print.o
	avr-gcc $^ -mmcu=atmega328p -Os -o $@

firmware.bin: firmware.elf
	avr-objcopy -O binary $< $@

flash: firmware.bin
	avrdude -c arduino -P /dev/ttyUSB0 -p m328p -U flash:w:$<
