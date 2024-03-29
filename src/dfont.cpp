// $Id: dfont.cpp,v 1.3 2003/04/14 05:51:03 mbridak Exp $

#include "dfont.h"
#include "files.h"

#include <fstream>
using std::ios;
using std::ofstream;
using std::ifstream;

DFont::DFont(const char *IFile, const char *DFile)
{
    fontSurface = Load_Image(IFile);

    ifstream inCredit(DFile, ios::in);
    if (!inCredit)
        initMap(DFile);  // if the font file doesn't exist then create one
    else
        loadMap(DFile);

    loadMap(DFile);
    gap = fonts['!'].w;
}

DFont::~DFont()
{
    SDL_FreeSurface(fontSurface);
}

void DFont::PutString(SDL_Surface *screen, Sint16 x, Sint16 y, const char *str)
{
    for ( ; *str != '\0'; ++str) {
        if (*str == ' ')
            x += gap;
        else {
            PutChar(screen, x, y, *str);
            x += fonts[*str].w;
        }
    }
}

void DFont::PutChar(SDL_Surface *screen, Sint16 x, Sint16 y, char c)
{
    SDL_Rect dest;
    dest.w = fonts[c].w;
    dest.h = fonts[c].h;
    dest.x = x;
    dest.y = y;
    SDL_BlitSurface(fontSurface, &fonts[c], screen, &dest);
}

/*
This function used to save our newly created fonts to a file.
This is a bad idea as we do not know where the file is being
created. It usually ends up in ./data/ and that is not
nice behaviour. Commenting out the file creation and
writes to the file to avoid this. We ship with the proper
font files in the tarball, that should be enough.
-- Jesse
*/
void DFont::initMap(const char * name)
{
    // ofstream file(name);
    // Sentry value
    Uint32 sentry = SDL_MapRGB(fontSurface->format, 255, 0, 255);
    char index = '!';  // Start of the fonts in ASCII
    // Uint16 w;
    Sint16 x(0);
    // Read the file from left to right detecting the font break markers along the way
    while (x < fontSurface->w) {
        if (GetPixel(fontSurface, x, 0) != sentry) {
            fonts[index].x = x;
            fonts[index].y = 1;
            fonts[index].h = fontSurface->h;
            for ( ; GetPixel(fontSurface, x, 0) != sentry && x < fontSurface->w; ++x)
                ;
            fonts[index].w = (x - fonts[index].x);
            // file << fonts[index];
            ++index;
        } else
            ++x;
    }
}

void DFont::loadMap(const char *name)
{
    ifstream file(name);
    char c = '!';  // all fonts must start with ! and then increment through the
                   // ASCII table!!
    while (file >> fonts[c++]) ;
}

Uint32 DFont::GetPixel(SDL_Surface *screen, Sint32 x, Sint32 y)
{
    switch (screen->format->BytesPerPixel) {
        case 1: {  /* Assuming 8-bpp */
            return *((Uint8 *)screen->pixels + y * screen->pitch + x);
        }
        break;

        case 2: {  /* Probably 15-bpp or 16-bpp */
            return *((Uint16 *)screen->pixels + y * screen->pitch / 2 + x);
        }
        break;

        case 3: {  /* Slow 24-bpp mode, usually not used */
            // Fixme
            return 0;
        }
        break;

        case 4: {  /* Probably 32-bpp */
            return *((Uint32 *)screen->pixels + y * screen->pitch / 4 + x);
        }
        break;
    }
    return 0xffffffff;
}

// Good Ol operator overloading Ahhhhh
ostream& operator << (ostream & os, SDL_Rect const & r)
{
    return os << r.x << ' ' << r.y << ' ' << r.w << ' ' << r.h << ' ';
}

istream& operator >> (istream & is, SDL_Rect & r)
{
    return is >> r.x >> r.y >> r.w >> r.h;
}
