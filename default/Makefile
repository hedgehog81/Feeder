###############################################################################
# Makefile for the project Feeder
###############################################################################

## General Flags
PROJECT = Feeder
MCU = atmega328p
TARGET = Feeder.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2                 -DF_CPU=16000000UL  -O2 -fsigned-char
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0


## Include Directories
INCLUDES = -I"D:\Stuff\avr\Feeder\Source\Feeder\board\minipro" -I"D:\Stuff\avr\Feeder\Source\Feeder\inc" 

## Objects that must be built in order to link
OBJECTS = main.o i2c.o rtc.o servo.o uart.o timer.o shell.o i2c_impl_bitbang.o settings.o feeder.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Feeder.hex Feeder.eep Feeder.lss size

## Compile
main.o: ../src/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

i2c.o: ../src/i2c.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rtc.o: ../src/rtc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

servo.o: ../src/servo.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart.o: ../src/uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer.o: ../src/timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

shell.o: ../src/shell.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

i2c_impl_bitbang.o: ../src/i2c_impl_bitbang.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

settings.o: ../src/settings.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

feeder.o: ../src/feeder.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) Feeder.elf dep/* Feeder.hex Feeder.eep Feeder.lss

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

