#ifndef __GOBJECT_H__
#define __GOBJECT_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct gObject gObject;

typedef enum {
    MOVE = 0,
    JUMP,
    SHOOT,
    LAST_ACTION
} eActions;

enum {
    CAN_MOVE = 0,
    CAN_JUMP,
    CAN_SHOOT,
    IS_DRAWN,
    LAST_ATTRIBUTE
} eAttributes;

typedef bool (*pactionf) (gObject* o);

struct gObject {
    unsigned int gid;
    
    bool factions[LAST_ACTION];
    pactionf* actions;
    bool* attributes;
    int x, y, w, h;
    int sx, sy, sw, sh;
    double move_timer;
    double move_period;
    int jump_total_steps;
    int jump_updates_per_step;
    int jump_updates;
    int jump_steps;
    int sindex;
    SDL_Surface* surface;
    SDL_Texture* texture;
};

typedef enum {
    PLAYER = 0,
    ENEMY,
    NPC,
    ENV,
    LAST_TYPE
} g_object_type;

bool move (gObject*);
bool jump (gObject*);
bool shoot(gObject*);

gObject* create_game_object(g_object_type t);

void set_pos_and_size(gObject* o, int x, int y, int w, int h);
void set_pos_and_size_in_surface(gObject* o, int x, int y, int w, int h);
bool load_surface(gObject* o, const char* s);
void create_texture(gObject* o, SDL_Renderer* r);
void free_object(gObject* o);
void step(gObject* o);
void object_render_copy(SDL_Renderer* render, gObject* o);
void set_action(gObject* o, eActions a);

#endif
