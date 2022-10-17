CC = cc
CFLAGS =
LDFLAGS =

SRC = themize.c
OBJ = ${SRC:.c=.o}

all: themize

.c.o:
	${CC} -c ${CFLAGS} $<

themize: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	${RM} themize ${OBJ}

.PHONY: all clean
