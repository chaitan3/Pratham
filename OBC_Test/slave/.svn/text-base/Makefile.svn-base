MCU = atmega128

PRJDIR = $(shell dirname `pwd`)

PROGRAMMER = avrispmkII

PROJ = ../tests/slavetest

TARGET = $(PROJ).out

INCLUDE = -I$(PRJDIR) -I$(PRJDIR)/slave -I$(PRJDIR)/interfaces -I$(PRJDIR)/cc1020

CFLAGS = -g -mmcu=$(MCU) -Os \
	$(INCLUDE) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall -Wstrict-prototypes               \
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))
	
LDFLAGS=-Wl,-Map,$(TARGET).map -mmcu=$(MCU) -lm
	
OBJ = $(PROJ).o ax25.o eeprom.o ../cc1020/transmit.o ../interfaces/spi.o 

LST = $(OBJ:.o=.lst)

all: $(TARGET)

$(TARGET): $(OBJ)
	avr-gcc $(OBJ) -o $(TARGET) $(LDFLAGS)
    
%.o: %.c
	avr-gcc $(CFLAGS) -c $< -o $@

%.hex: %.out
	avr-objcopy -O ihex $< $@

program: $(PROJ).hex
	avrdude -c $(PROGRAMMER) -P usb -p atmega128 -U flash:w:$<

clean:
	rm -f $(OBJ) $(LST) 
	rm -f $(PROJ).hex $(TARGET) $(TARGET).map
size: $(TARGET)
	avr-size $(TARGET)

