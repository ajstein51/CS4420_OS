CC=gcc
CFLAGS=-Wall -Werror -O3 -g
TARGET=main

default:
	@echo "Make not here"
	@exit 1

uniq: $(TARGET).c 
	$(CC) $(CFLAGS) -o uniq $(TARGET).c

clean:
	$(RM) uniq