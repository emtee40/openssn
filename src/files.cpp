#include <stdio.h>
#include <string.h>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "files.h"

static const char *data_dir = NULL;

// Find a file's full path based on the name of a file
char *Find_Data_File(const char *filename)
{
    char *location;
    FILE *my_file;

    // allocate the path string which will be returned
    location = (char *) calloc(strlen(filename) + 64, sizeof(char));
    if (!location)
        return NULL;

    // maybe we already know the directory and don't have to search
    if (data_dir) {
        sprintf(location, "%s/%s", data_dir, filename);
        return location;
    }

    // next chance, try DATADIR
    sprintf(location, "%s/%s", DATADIR, filename);
    my_file = fopen(location, "r");
    if (my_file) {
        fclose(my_file);
        data_dir = DATADIR;
        return location;
    }

    // last chance, try OPTDIR
    sprintf(location, "%s/%s", OPTDIR, filename);
    my_file = fopen(location, "r");
    if (my_file) {
        fclose(my_file);
        data_dir = OPTDIR;
        return location;
    }

    // if all the above failed, use the local dir
    data_dir = ".";
    sprintf(location, "%s/%s", data_dir, filename);
    return location;
}

SDL_Surface *Load_Image(const char *filename)
{
    char *file_name;
    SDL_Surface *my_image;
    SDL_Surface *tmp_image;

    // Find full path for the file
    file_name = Find_Data_File(filename);
    if (!file_name) {
        std::cerr << "Failed to find path"
                  << "for image file " << filename << std::endl
                  << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(0);
    }

    // Load the image
    tmp_image = IMG_Load(file_name);
    free(file_name);
    if (!tmp_image) {
        std::cerr << "Failed to load image file " << filename << std::endl
                  << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(0);
    }

    // Convert the image to the display format
    my_image = SDL_DisplayFormat(tmp_image);
    if (!my_image) {
        std::cerr << "Failed to convert image file " << filename
                  << "to display format" << std::endl
                  << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(0);
    }
    SDL_FreeSurface(tmp_image);

    return my_image;
}
