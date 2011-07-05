CC = g++
PREFIX ?= /usr
DEVCFLAGS = -Wall -g -D_GNU_SOURCE `sdl-config --cflags` 
CFLAGS = -g -Wall `sdl-config --cflags` -Wno-write-strings # -DAIDEBUG # -DDEBUG
LIBS = -lm -lSDL_gfx -lSDL_image `sdl-config --libs`
OBJS = 	main.o sonar.o radar.o esm.o control.o submarine.o adcap.o coord.o stack.o contact.o towedarray.o targetmotionanalysis.o boatpositionlog.o dfont.o dstack.o message.o gameclock.o files.o mission.o menu.o draw.o
SSN = ../openssn
DATADIR = $(PREFIX)/share/games/openssn


.SUFFIXES: .cpp .o

.cpp.o:	
	$(CC) $(CFLAGS) -DVERSION=$(VERSION) -DDATADIR=\"$(DATADIR)\" -c $<

.o:
	$(CC) $(CLFAGS) $< -o $@

all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(SSN)

