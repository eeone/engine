#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MILLION 1000000.0
#define BILLION 1000000000.0

#define UPDATE_STEP 0.005
#define FRAME_SKIP 0.1
#define SPRITE_UPDATE 0.09;
#define CHECKPOINT 2.0

#define EPSILON 1E-7

#define SET_RECT(r, a, b, c, d) (r.x = a; r.y = b; r.w = c; r.h = d;)

#endif
