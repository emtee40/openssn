# Pre-compiled Binaries: 
+ **https://sourceforge.net/projects/openssn/files/**

OpenSSN - What is OpenSSN?
==========================

OpenSSN is a submarine simulation (subsim) which tries to emulate
the behaviour of modern submarines. The player is placed in command
of a submarine and is able to move about in a deep ocean environment.

Combat missions are now being added to OpenSSN, and ships with
sonar will run from torpedoes and, if they can detect the player,
will return fire.



Why a subsim?
======================

I've always been a fan of subsims and played commercial games,
but couldn't find a modern subsim for open source operating systems
(such as Linux and FreeBSD). Danger from the Deep is available,
but it's stictly for World War II u-boats, and has relatively high
graphics requirements. Looking around I found a half dozen or so
subsims that had begun and dropped away shortly after beginning development.

The LinuxSSN project had laid a solid foundation before the developers
dropped the project and I decided to pick up where they left off.
The heart of OpenSSN comes from their project and attempts to further
their work.




Licensing info
====================

The OpenSSN project is licensed under the GNU General Public License,
version 2. The LinuxSSN project was also GNU GPL licensed and we
carry on that tradition. Copyright Jesse Smith <jessefrgsmith@yahoo.ca>
Please see the LICENSE file for further details.

Data files, missions, artwork and images included in OpenSSN are
licensed under the Creative Commons CC0 1.0 Universal license. 
Please see the ART-LICENSE file for full details.




What do I need to build OpenSSN?
=================================

To build OpenSSN you'll need a C++ compiler (GNU's Compiler Collection
is recommended) and the following development libraries:

SDL
SDL_gfx
SDL_image
SDL_mixer

Assuming the above libraries are installed, it should be possible to
build OpenSSN with the following commands

tar zxf openssn-1.4.tar.gz
cd openssn
make





Running OpenSSN
==================================

Once OpenSSN has built, an executable file (called openssn) will
be placed in the top level directory. From there
the game can be run with the command:

./openssn

The following command line parameters are recognized:

-w		Run in windowed mode, this is the default
-f		Run in full screen
-s		Enable sound effects
-v		Display version number and exit
-h		Display help and exit
-m <mission> 	Load a specific mission




Control Keys
===========================

The player's submarine can be controlled with either the mouse 
or keyboard commands. The following key commands are recognized.

Pilot the submarine..............
Plus key		Increase sub's speed
Minus key		Slow down
Left keypad (4)		Turn left
Right keypad (6)	Turn right
Down keypad (2)		Dive deeper
Up keypad (8)		Rise toward the surface
Q			Go up one thermal
A			Hold depth here
Z			Go down one thermal
S			Surface
X			Go to periscope depth


System controls...................
F1			Sonar screen
F2			Map screen
F3			Weapons station
F4			Helm console
F5			ESM signals screen
F6			Radar screen
F10			Take screen shot
TAB			Select next target
Delete			Increase map scale
Insert			Decrease map scale
Up, down, left, right	Manipulate map
P			Pause the game
Page Up			Compress Time
Page Down		Slow time
ESC 			Quit



Weapon controls 
T			Load a torpedo
N			Load a noise maker
U			Unload a tube
F			Fire a tube

Each of the weapon station keys should be followed by
a number indication which tube should be loaded or fired.
The tubes are numbered 1-6.


Sonar controls
G			Send active sonar pulse.
E			Extend sonar array
R			Retract towed sonar array
W			Hold towed sonar in place




Contact information
============================

Did you find a bug? Would you like to request a feature? Did
you patch something and want to share it? Send an e-mail to
jessefrgsmith@yahoo.ca. Please place "OpenSSN" in the subject line.

