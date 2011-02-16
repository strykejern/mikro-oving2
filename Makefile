#Simple makefile for assignment 2
BINARY = main.elf

#Compiling phase
compile: 	src/main.c src/io.c
		avr32-gcc -Wall -g -c $^
		avr32-gcc -o $(BINARY) main.o io.o

#Cleanup object files and binaries
clean:		
		rm -rf *.o *.elf
		make compile

#Do everyting
all:
		make clean
		make compile
		make upload
		make debug

#Upload the binary to the microcontroller
.PHONY: 	upload
upload:		$(BINARY)
		avr32program halt
		sleep 3
		avr32program program -f0,8Mb -e $(BINARY)
		sleep 2


.PHONY: 	debug
debug:		$(BINARY)
		avr32gdbproxy -f 0,8MB -a remote:1024 &
		sleep 1
		avr32-gdb $(BINARY) -eval-command="target remote:1024"
