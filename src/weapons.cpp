#include "weapons.h"
#include "dfont.h"
#include "draw.h"
#include "files.h"

Weapons::Weapons()
{
}

Weapons::~Weapons()
{
}

void Weapons::setSubs(Submarine *Subs)
{
    this->Subs = Subs;
}

void Weapons::InitGraphics(SDL_Surface *screen)
{
    this->screen = screen;

    black = SDL_MapRGB(screen->format, 0, 0, 0);
    green = SDL_MapRGB(screen->format, 0, 255, 0);
}

void Weapons::LoadWidgets()
{
    weaponsconsole.load("images/Mapscreen.png");
    noisemaker_image.load("images/noisemaker.png");
    torpedo_image.load("images/torpedo.png");
}

void Weapons::UnLoadWidgets()
{
    // free the surfaces
    weaponsconsole.unload();
    torpedo_image.unload();
    noisemaker_image.unload();
}

void Weapons::DisplayWidgets()
{
    SDL_Rect weapons, tubes;
    static DFont fnt("images/font.png", "data/font.dat");
    char text[256];
    int index, y1, y2;

    weaponsconsole.draw(screen, 0, 0);

    weapons.x = 150;
    weapons.y = 145;
    weapons.w = 200;
    weapons.h = 300;
    tubes.x = 374;
    tubes.y = 145;
    tubes.h = 500;
    tubes.w = 501;

    SDL_FillRect(screen, &weapons, black);
    sprintf(text, "WEAPONS");
    fnt.PutString(screen, 190, 150, text);
    sprintf(text, "     Torpedos: %d", Subs->TorpedosOnBoard);
    fnt.PutString(screen, 160, 174, text);
    sprintf(text, "Noise Makers: %d", Subs->NoiseMakers);
    fnt.PutString(screen, 160, 196, text);
    sprintf(text, "'T' to load torpedo");
    fnt.PutString(screen, 160, 300, text);
    sprintf(text, "'N' to load noise maker");
    fnt.PutString(screen, 160, 312, text);
    sprintf(text, "'U' to unload device");
    fnt.PutString(screen, 160, 324, text);
    sprintf(text, "'F' to fire");
    fnt.PutString(screen, 160, 336, text);

    SDL_FillRect(screen, &tubes, black);
    y1 = 150;
    y2 = 190;
    for (index = 0; index < MAX_TUBES; index++) {
        // draw tube
        DrawRectangle(screen, 390, y1, 500, y2, green);
        // draw buttons
        fnt.PutString(screen, 520, y1 + 5, "  Load");
        fnt.PutString(screen, 520, y1 + 18, "Torpedo");
        DrawRectangle(screen, 520, y1, 590, y2, green);
        fnt.PutString(screen, 600, y1 + 5, "   Load");
        fnt.PutString(screen, 600, y1 + 18, "Noise Maker");
        DrawRectangle(screen, 600, y1, 690, y2, green);
        fnt.PutString(screen, 700, y1 + 10, "Unload");
        DrawRectangle(screen, 700, y1, 760, y2, green);
        fnt.PutString(screen, 770, y1 + 10, "Fire!");
        DrawRectangle(screen, 770, y1, 815, y2, green);
        y1 += 50;
        y2 += 50;
    }

    // put stuff in the tubes
    y1 = 151;
    for (index = 0; index < MAX_TUBES; index++) {
        if (Subs->torpedo_tube[index] == TUBE_TORPEDO)
            torpedo_image.draw(screen, 391, y1);
        else if (Subs->torpedo_tube[index] == TUBE_NOISEMAKER)
            noisemaker_image.draw(screen, 391, y1);
        else {
            sprintf(text, "Tube %d", index + 1);
            fnt.PutString(screen, 420, y1 + 10, text);
        }
        y1 += 50;
    }
}

void Weapons::UpdateDisplay()
{
    DisplayWidgets();
}
