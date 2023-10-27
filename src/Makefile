CXX ?= clang++
PREFIX ?= /usr
DEVCFLAGS = -Wall -g -D_GNU_SOURCE `sdl-config --cflags` 
CFLAGS = -O2 -Wall `sdl-config --cflags` -Wno-write-strings \
# -DDEBUG_HELICOPTER # -DDEBUGMAP # -DAIDEBUG # -DDEBUG 
LIBS = -lm -lSDL_gfx -lSDL_image -lSDL_mixer `sdl-config --libs`
OBJS = 	main.o sonar.o navigation.o weapons.o control.o esm.o radar.o submarine.o adcap.o coord.o stack.o contact.o towedarray.o targetmotionanalysis.o boatpositionlog.o dfont.o dstack.o message.o gameclock.o files.o mission.o menu.o draw.o sound.o map.o winfunctions.o helicopter.o widget.o
SSN = ../openssn
DATADIR = $(PREFIX)/share/games/openssn
VERSION ?= 0.0

.SUFFIXES: .cpp .o

.cpp.o:	
	$(CXX) $(CFLAGS) -DVERSION=$(VERSION) -DDATADIR=\"$(DATADIR)\" -DOPTDIR=\"$(OPTDIR)\" -c $<

.o:
	$(CXX) $(CLFAGS) $< -o $@

all: $(SSN)
$(SSN): $(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o $(SSN)

