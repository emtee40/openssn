#include <stdio.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "files.h"


// Find a file's full path based on the name of a file
char *Find_Data_File(char *filename)
{
   char *location;
   FILE *my_file;

   location = (char *) calloc( strlen(filename) + 64, sizeof(char) );
   if (! location)
      return filename;

   sprintf(location, "/usr/local/share/games/openssn/%s", filename);
   my_file = fopen(location, "r");
   if (my_file)
   {
       fclose(my_file);
       return location;
   }

   sprintf(location, "/usr/share/games/openssn/%s", filename);
   my_file = fopen(location, "r");
   if (my_file)
   {
      fclose(my_file);
      return location;
   }
   
   return filename;
}


SDL_Surface *Load_Image(char *filename)
{
   char *file_name;
   SDL_Surface *my_image;

   file_name = Find_Data_File(filename);
   if (file_name)
      my_image = IMG_Load(file_name);
   if ( (file_name) && (file_name != filename) )
       free(file_name);
   return my_image;

}


