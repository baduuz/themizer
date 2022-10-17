CC = cc
CFLAGS = -Wall -O2 -std=c99 -pedantic
LDFLAGS = -lm

SRC = themize.c util.c
OBJ = $(SRC:.c=.o)

all: themize

themize: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $<

$(OBJ): config.h util.h

config.h:
	cp config.def.h $@

clean:
	$(RM) themize $(OBJ)

.PHONY: all clean
