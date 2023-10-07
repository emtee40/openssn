// $Id: dfont.h,v 1.3 2003/04/14 05:51:03 mbridak Exp $

#ifndef DFONT_H
#define DFONT_H

#include <iostream>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

using std::istream;
using std::ostream;

class DFont
{
public:
    DFont(const char *, const char *);
    ~DFont();
    void PutString(SDL_Surface *, Sint16, Sint16, const char *);
    void PutChar(SDL_Surface *, Sint16, Sint16, char);

protected:
    std::map <char, SDL_Rect> fonts;
    SDL_Surface *fontSurface;
    int gap;

private:
    void initMap(const char *);
    void loadMap(const char *);
    Uint32 GetPixel(SDL_Surface *, Sint32, Sint32);
};

ostream& operator << (ostream &, SDL_Rect const &);  // output a record
istream& operator >> (istream &, SDL_Rect &);  // input a record

#endif  // DFONT_H
