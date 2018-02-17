/*

gcc texture_converter.c -Wall -lSDL -lSDL_image -lTIFF -ljpeg -lpng -lwebp -lz -lpthread

*/

#include <stdio.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

void convert(SDL_Surface* s, FILE* f)
{
    int x, y;

    Uint32 rmask = s->format->Rmask;
    Uint32 gmask = s->format->Gmask;
    Uint32 bmask = s->format->Bmask;
    Uint32 amask = s->format->Amask;

    Uint32 rshift = s->format->Rshift;
    Uint32 gshift = s->format->Gshift;
    Uint32 bshift = s->format->Bshift;
    Uint32 ashift = s->format->Ashift;

    printf("Surface w %d, h %d, bits %d, pitch %d\n",
        s->w, s->h, s->format->BitsPerPixel, s->pitch);

    printf("rm %x, gm %x, bm %x am %x\n", rmask, gmask, bmask, amask);
    printf("rs %d, gs %d, bs %d as %d\n", rshift, gshift, bshift, ashift);

    for (y = 0; y < s->h; y++) {
        Uint32 *ptr = (Uint32*)((char*)s->pixels + y * s->pitch);

        for (x = 0; x < s->w; x++) {
            Uint32 c = ptr[x];

            Uint32 r = (c & rmask) >> rshift;
            Uint32 g = (c & gmask) >> gshift;
            Uint32 b = (c & bmask) >> bshift;
            Uint32 a = (c & amask) >> ashift;

            c = r << 24 | g << 16 | b << 8 | a;

            c = SDL_SwapBE32(c);

            fwrite(&c, sizeof(c), 1, f);
#if 0
            static Uint32 last = 0;

            if (c != last) {
                printf("Wrote 0x%x\n",c);
                last = c;
            }
#endif
        }
    }
}

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {

        if (IMG_Init(IMG_INIT_PNG)) {

            SDL_Surface* surface = IMG_Load("Bitmap/atlas.png");

            if (surface) {
                FILE* file = fopen("atlas.bin", "wb");

                if (file) {
                    convert(surface, file);
                    fclose(file);
                } else {
                    printf("fopen failed\n");
                }

                SDL_FreeSurface(surface);
            } else {
                printf("IMG_Load failed\n");
            }

            IMG_Quit();
        } else {
            printf("IMG_Init failed\n");
        }

        SDL_Quit();
    } else {
        printf("SDL_Init failed\n");
    }

    return 0;
}
