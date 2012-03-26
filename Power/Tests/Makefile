TARGET = final3
all: $(TARGET).hex

$(TARGET).hex:$(TARGET).asm
	gpasm $(TARGET).asm

clean:
	rm -f $(TARGET).hex $(TARGET).lst $(TARGET).cod
program: $(TARGET).hex
	sudo pk2cmd -PPIC16F877A -M -F$(TARGET).hex -Y
	sudo pk2cmd -PPIC16F877A -R
sim:
	gpsim -s $(TARGET).cod $(TARGET).asm
