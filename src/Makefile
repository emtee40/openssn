CC = g++
DEVCFLAGS = -Wall -g -D_GNU_SOURCE `sdl-config --cflags` 
CFLAGS = -g -Wall `sdl-config --cflags` -Wno-write-strings # -DDEBUG
LIBS = -lm -lSDL_gfx -lSDL_image `sdl-config --libs`
OBJS = 	main.o sonar.o radar.o esm.o control.o submarine.o adcap.o coord.o stack.o contact.o towedarray.o targetmotionanalysis.o boatpositionlog.o dfont.o dstack.o message.o gameclock.o files.o
SSN = ../openssn
PREFIX ?= /usr
DATADIR = $(PREFIX)/share/games/openssn


.SUFFIXES: .cpp .o

.cpp.o:	
	$(CC) $(CFLAGS) -c $<

.o:
	$(CC) $(CLFAGS) $< -o $@

all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(SSN)

install: all
	mkdir -p $(PREFIX)/bin
	mkdir -p $(DATADIR)/data
	mkdir -p $(DATADIR)/images
	cp $(SSN) $(PREFIX)/bin/
	cp ../images/* $(DATADIR)/images
	cp ../data/* $(DATADIR)/data

deinstall:
	rm -f $(PREFIX)/bin/openssn
	rm -rf $(DATADIR)

clean:
	rm -f *.o *~ core $(SSN)

clobber: clean
	rm -f $(SSN)

dist:
	rm -f linuxssn.tgz
	tar cvzf linuxssn.tgz *.h *.cpp makefile CHANGES Authors README STATUS USAGE *.png *.dat *.doc

printout:
	a2ps -g -4 ./*.cpp -o source.ps
	a2ps -g -4 ./*.h -o headers.ps
	ps2pdf source.ps
	ps2pdf headers.ps
	rm ./*.ps


