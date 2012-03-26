TARGET = iotest
all: $(TARGET).hex

$(TARGET).hex:$(TARGET).asm
	gpasm $(TARGET).asm

clean:
	rm $(TARGET).hex $(TARGET).lst $(TARGET).cod
program:
	sudo pk2cmd -PPIC16F877A -M -F$(TARGET).hex -Y
	sudo pk2cmd -PPIC16F877A -A5 -T -R
sim:
	gpsim -s $(TARGET).cod $(TARGET).asm
