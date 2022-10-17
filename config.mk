VERSION = 0.1

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

# includes and libs
INCS =
LIBS = -lm

# flags
CFLAGS   = -std=c99 -pedantic -Wall -O2 $(INCS) $(CPPFLAGS) -DVERSION=\"$(VERSION)\"
LDFLAGS  = $(LIBS)

# compiler and linker
CC = cc
