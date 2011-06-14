PREFIX ?= /usr
SSN = openssn
DATADIR = $(PREFIX)/share/games/openssn

all:
	$(MAKE) -C src

clean:
	rm -f src/*.o $(SSN)

install: all
	mkdir -p $(PREFIX)/bin
	mkdir -p $(DATADIR)/data
	mkdir -p $(DATADIR)/images
	cp $(SSN) $(PREFIX)/bin/
	cp images/* $(DATADIR)/images
	cp data/* $(DATADIR)/data

deinstall:
	rm -rf $(PREFIX)/bin/$(SSN)
	rm -rf $(DATADIR)
