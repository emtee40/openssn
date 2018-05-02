PREFIX ?= /usr
SSN = openssn
DATADIR = $(PREFIX)/share/openssn
OPTDIR = /opt/openssn
VERSION = 1.4

all:
	$(MAKE) VERSION=$(VERSION) PREFIX=$(PREFIX) OPTDIR=$(OPTDIR) DATADIR=$(DATADIR) -C src
	gzip -c openssn.6 > openssn.6.gz

clean:
	rm -f src/*.o $(SSN)
	rm -f openssn.6.gz

windows:
	$(MAKE) -C src -f Makefile.windows

install: all
	mkdir -p $(PREFIX)/bin
	mkdir -p $(DATADIR)/data
	mkdir -p $(DATADIR)/images
	mkdir -p $(DATADIR)/ships
	mkdir -p $(DATADIR)/sounds
	mkdir -p $(PREFIX)/share/pixmaps
	mkdir -p $(PREFIX)/share/applications
	mkdir -p $(PREFIX)/share/man/man6/
	cp $(SSN) $(PREFIX)/bin/
	cp images/* $(DATADIR)/images
	cp data/* $(DATADIR)/data
	cp ships/* $(DATADIR)/ships
	cp sounds/* $(DATADIR)/sounds
	cp openssn.png $(PREFIX)/share/pixmaps
	cp openssn.desktop $(PREFIX)/share/applications
	cp openssn.6.gz $(PREFIX)/share/man/man6

optinstall: all
	mkdir -p $(OPTDIR)
	mkdir -p $(OPTDIR)/data
	mkdir -p $(OPTDIR)/images
	mkdir -p $(OPTDIR)/ships
	mkdir -p $(OPTDIR)/sounds
	mkdir -p $(PREFIX)/share/applications
	mkdir -p $(PREFIX)/share/pixmaps
	cp $(SSN) $(OPTDIR)
	cp images/* $(OPTDIR)/images
	cp data/* $(OPTDIR)/data
	cp ships/* $(OPTDIR)/ships
	cp sounds/* $(OPTDIR)/sounds
	cp openssn.desktop $(PREFIX)/share/applications/
	cp openssn.png $(PREFIX)/share/pixmaps/

deinstall:
	rm -rf $(PREFIX)/bin/$(SSN)
	rm -rf $(DATADIR)
	rm -rf $(PREFIX)/share/man/man6/openssn.6.gz
	rm -f $(PREFIX)/share/applications/openssn.desktop
	rm -f $(PREFIX)/share/pixmaps/openssn.png

optdeinstall:
	rm -rf $(OPTDIR)
	rm -rf $(PREFIX)/share/applications/openssn.desktop
	rm -rf $(PREFIX)/share/pixmaps/openssn.png

tarball: clean
	cd .. && tar czf openssn-$(VERSION).tar.gz openssn --exclude=.svn

zipfile: clean
	cd .. && zip -r openssn-$(VERSION)-source.zip openssn --exclude \*.svn\*

