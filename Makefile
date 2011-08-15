PREFIX ?= /usr
SSN = openssn
DATADIR = $(PREFIX)/share/games/openssn
VERSION = 0.7

all:
	$(MAKE) VERSION=$(VERSION) PREFIX=$(PREFIX) -C src

clean:
	rm -f src/*.o $(SSN)

windows:
	$(MAKE) -C src -f Makefile.windows

install: all
	mkdir -p $(PREFIX)/bin
	mkdir -p $(DATADIR)/data
	mkdir -p $(DATADIR)/images
	mkdir -p $(DATADIR)/ships
	mkdir -p $(DATADIR)/sounds
	cp $(SSN) $(PREFIX)/bin/
	cp images/* $(DATADIR)/images
	cp data/* $(DATADIR)/data
	cp ships/* $(DATADIR)/ships
	cp sounds/* $(DATADIR)/sounds

deinstall:
	rm -rf $(PREFIX)/bin/$(SSN)
	rm -rf $(DATADIR)
