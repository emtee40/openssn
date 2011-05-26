#ifndef FILES_HEADER_FILE__
#define FILES_HEADER_FILE__
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

// Find the full path of a file based on its name.
// The return value should be freed UNLESS the return
// value is equal to the filename passed in.
char *Find_Data_File(char *filename);


// Try to find the image file in various locations
SDL_Surface *Load_Image(char *filename);


#endif

