CC = gcc
CFLAGS = -Wall
DEPS = emulator_parser.h
OBJ = emulator.o emulator_parser.o client.o 

all: client emulator
	chmod +x $^

debug: emulator.c client.c	
	@echo "$:@ print the target name" $@
	@echo "$:^ print all prerequisites" $^
	@echo "$:< print the first prerequisite" $<
	@echo "$:? print all prerequisites newer than the target" $?

client: client.o
	$(CC) -o $@ $^ $(CFLAGS)

emulator: emulator.o emulator_parser.o
	$(CC) -o $@ $^ $(DEPS) $(CFLAGS)

.PHONY : clean
clean: 
	rm -rf *.o emulator client


