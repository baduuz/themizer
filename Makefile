include config.mk

SRC = themize.c util.c
OBJ = $(SRC:.c=.o)

all: options themize

options:
	@echo themize build options:
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo "CC       = $(CC)"

.c.o:
	$(CC) -c $(CFLAGS) $<

config.h:
	cp config.def.h $@

$(OBJ): config.h util.h

themize: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	$(RM) themize $(OBJ) themize-$(VERSION).tar.gz

dist: clean
	mkdir -p themize-$(VERSION)
	cp Makefile config.mk config.def.h gruvbox_palette.txt nord_palette.txt\
		stb_image.h stb_image_write.h $(SRC) util.h themize-$(VERSION)
	tar -czf themize-$(VERSION).tar.gz themize-$(VERSION)
	$(RM) -r themize-$(VERSION)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f themize $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/themize

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/themize

.PHONY: all clean dist options install uninstall
