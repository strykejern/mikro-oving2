#Simple makefile for assignment 2
BINARY = main.elf

#Compiling phase
compile: 	src/main.c src/io.c
		avr32-gcc -Wall -g -c $^
		avr32-gcc -o $(BINARY) main.o io.o

#Cleanup object files and binaries
clean:		
		rm -rf *.o

#Do everyting
all:
		make upload
		make debug
		make clean

#Upload the binary to the microcontroller
.PHONY: 	upload
upload:		$(BINARY)
		make compile
		avr32program halt
		sleep 3
		avr32program program -r -R -f0,8Mb -e $(BINARY)


.PHONY: 	debug
debug:		$(BINARY)
		sleep 2
		avr32gdbproxy -f 0,8MB -a remote:1024 &
		sleep 1
		avr32-gdb -eval-command="file main.elf" -eval-command="target remote:1024"
