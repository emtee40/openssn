PREFIX ?= /usr
SSN = openssn
DATADIR = $(PREFIX)/share/openssn
VERSION = 0.9

all:
	$(MAKE) VERSION=$(VERSION) PREFIX=$(PREFIX) -C src
	gzip -c openssn.6 > openssn.6.gz

clean:
	rm -f src/*.o $(SSN)
	rm openssn.6.gz

windows:
	$(MAKE) -C src -f Makefile.windows

install: all
	mkdir -p $(PREFIX)/bin
	mkdir -p $(DATADIR)/data
	mkdir -p $(DATADIR)/images
	mkdir -p $(DATADIR)/ships
	mkdir -p $(DATADIR)/sounds
	mkdir -p $(PREFIX)/share/pixmaps
	mkdir -p $(PREFIX)/share/applications/openssn.desktop
	mkdir -p $(PREFIX)/share/man/man6/
	cp $(SSN) $(PREFIX)/bin/
	cp images/* $(DATADIR)/images
	cp data/* $(DATADIR)/data
	cp ships/* $(DATADIR)/ships
	cp sounds/* $(DATADIR)/sounds
	cp openssn.png $(PREFIX)/share/pixmaps
	cp openssn.desktop $(PREFIX)/share/applications/openssn.desktop
	cp openssn.6.gz $(PREFIX)/share/man/man6

deinstall:
	rm -rf $(PREFIX)/bin/$(SSN)
	rm -rf $(DATADIR)
	rm -rf $(PREFIX)/share/man/man6/openssn.6.gz
	rm $(PREFIX)/share/applications/openssn.desktop
	rm $(PREFIX)/share/pixmaps/openssn.png

tarball: 
	cd .. && tar czf openssn-$(VERSION).tar.gz openssn

