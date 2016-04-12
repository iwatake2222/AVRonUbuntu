# Setup Guilde for AVR (ATmega) Development on Ubuntu

## Environment
* Host PC = Ubuntu 15.10
* AVR Writer = Pololu USB AVR Programmer
* Target Device = ATmega 328P
* Language = C/C++

## Get toolchains
* You can get packages using the following commands
```
sudo apt-get install binutils-avr gcc-avr avr-libc
```

* However, I would like to get "Official" Atmel AVR Toolchain for Linux from here: <http://www.atmel.com/tools/atmelavrtoolchainforlinux.aspx>
	* Atmel AVR 8-bit Toolchain 3.5.1 - Linux 64-bit
* Extract it here
	* ~/avr/avr8-gnu-toolchain-linux_x86_64
* Modify path
	* PATH=$PATH:~/avr/avr8-gnu-toolchain-linux_x86_64/bin/
	* better to add it in .bashrc

## Build (command line)
* Build source file using the following commands
	* Note: you don't need to convert obj format into ihex because AVRDUDE now accepts ELF format
```
# Compile
avr-gcc -g -c -O2 -mmcu=atmega328p -DF_CPU=16000000UL main.c -o main.o

# Link
avr-gcc -g -O2 -mmcu=atmega328p main.o -o hello.out

# OBJ
#avr-objcopy -j .text -j .data -O ihex hello hello.ihex
```

* sample: main.c
```
#include <avr/io.h>
#include <util/delay.h>

#define DDR_LED  DDRB
#define PORT_LED PORTB
#define BIT_LED  5


int main(void)
{
	DDR_LED |= 1 << BIT_LED;

	while(1){
		PORT_LED |= 1 << BIT_LED;
		_delay_ms(200);
		PORT_LED &= ~(1 << BIT_LED);
		_delay_ms(200);
	}
}
```

## Build (makefile)
* When your project becomes bigger, you should use makefile
* sample makefile
```
TARGET = hello.out
MCU = atmega328p
F_CPU = 16000000UL
DIR_BUILD = build
MAKE = make
CC = avr-gcc
CXX = avr-g++
AR = avr-ar
ASM = avr-as
OBJCOPY = avr-objcopy
CFLAGS = -g -O2 -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS =  -g -O2 -Wall -mmcu=$(MCU)
INCLUDES = 
LIBS = 
SUBS = 
SUBDIRS = 
#SRCS = main.c sub.c
#OBJS = $(SRCS:.c=.o)
OBJS = main.o sub1.o sub2dir/sub2.o


.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(DIR_BUILD)/$(notdir $(basename $<)).o
.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $(DIR_BUILD)/$(notdir $(basename $<)).o

.PHONY: all
all: $(DIR_BUILD) $(TARGET)

$(DIR_BUILD):
	mkdir -p $(DIR_BUILD)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(addprefix $(DIR_BUILD)/, $(notdir $(OBJS))) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o *.a *.exe *.out
	@for subdir in $(SUBDIRS) ; do \
		(cd $$subdir && $(MAKE) clean) ;\
	done
	rm -rf $(DIR_BUILD)
```

## Write into the device
* install AVRDUDE
```
sudo apt-get install avrdude
```

* Find out COM port for your programmer
	* type the following commands. Here, /dev/ttyACM0 is your programmer port. /dev/ttyACM1 is for instant oscilloscope (SLO-scope)
```
# Do not connect the programmer to PC
ls /dev/ > temp
# Connect the programmer to PC
ls /dev/ > temp2
diff temp temp2
91a92
> serial
163a165,166
> ttyACM0
> ttyACM1
```

* Connect the programmer and device
	* PC - programmer
	* programmer - device
	* device - power supply

* Check connection
	* Type the following command. It should return device information.
```
sudo avrdude -c avrispv2 -P /dev/ttyACM0 -p m328p -v
```

* Write to flash memory
```
sudo avrdude -c avrispv2 -P /dev/ttyACM0 -p m328p -U flash:w:hello.out:e
```

* if failed, disconnect and connect everything. also you can change baudrate.
```
sudo avrdude -c avrispv2 -b9600 -P /dev/ttyACM0 -p m328p -U flash:w:hello.out:e
```

## Tips
### strip
```
avr-strip hello.out
```