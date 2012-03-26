MCU = atmega128

PROGRAMMER = avrispmkII

PRJDIR = $(shell pwd)

PROJ = test

TARGET = $(PROJ).out

INCLUDE = -I$(PRJDIR) \
          -I$(PRJDIR)/sensors -I$(PRJDIR)/interfaces -I$(PRJDIR)/cc1020 \
          -I$(PRJDIR)/controls -I$(PRJDIR)/slave

CFLAGS = -g -mmcu=$(MCU) -Os \
	$(INCLUDE) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall -Wstrict-prototypes               \
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))
	
LDFLAGS= -Wl,-Map,$(TARGET).map -mmcu=$(MCU) -Wl,-u,vfprintf -lprintf_flt -lm 
	
OBJ = hm.o comm.o $(PROJ).o peripherals.o slave_comm.o timer.o \
	 interfaces/spi.o interfaces/twi.o \
	 cc1020/functions.o \
	 sensors/mag.o sensors/sun.o sensors/gps.o \
	 slave/ax25.o \
	 controls/propagator.o controls/mathutil.o controls/frame.o controls/controller.o controls/igrf.o controls/quest.o

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
sig:
	avrdude -c $(PROGRAMMER) -P usb -p atmega128 -U signature:r
clean:
	rm -f $(OBJ) $(LST) 
	rm -f $(PROJ).hex $(TARGET) $(TARGET).map
size: $(TARGET)
	avr-size $(TARGET)
doc:
	doxygen Doxyfile
