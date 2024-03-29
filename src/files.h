#ifndef FILES_HEADER_FILE__
#define FILES_HEADER_FILE__

#include <SDL.h>
#include "SDL/SDL.h"

#ifndef DATADIR
#define DATADIR "."
#endif
#ifndef OPTDIR
#define OPTDIR "/opt/openssn"
#endif

// Find the full path of a file based on its name.
// The return value should be freed UNLESS the return
// value is equal to the filename passed in.
char *Find_Data_File(const char *filename);

// Try to find the image file in various locations
SDL_Surface *Load_Image(const char *filename);

#endif  // FILES_HEADER_FILE__
