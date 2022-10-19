include config.mk

SRC = themizer.c util.c
OBJ = $(SRC:.c=.o)

all: options themizer

options:
	@echo themizer build options:
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo "CC       = $(CC)"

.c.o:
	$(CC) -c $(CFLAGS) $<

config.h:
	cp config.def.h $@

$(OBJ): config.h util.h

themizer: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	$(RM) themizer $(OBJ) themizer-$(VERSION).tar.gz

dist: clean
	mkdir -p themizer-$(VERSION)
	cp Makefile config.mk config.def.h gruvbox_palette.txt nord_palette.txt\
		stb_image.h stb_image_write.h $(SRC) util.h themizer-$(VERSION)
	tar -czf themizer-$(VERSION).tar.gz themizer-$(VERSION)
	$(RM) -r themizer-$(VERSION)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f themizer $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/themizer

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/themizer

.PHONY: all clean dist options install uninstall
