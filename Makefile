CC=gcc
CFLAGS=-O2

default:
	$(CC) $(CFLAGS) main.c elf.c elf.h debug.c debug.h -o parser
	@./parser
