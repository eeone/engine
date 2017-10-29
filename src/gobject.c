#include "program.h"
#include "gobject.h"

static int lastid = 0;

bool move (gObject* o) {
    o->move_timer -= UPDATE_STEP;

    if (o->move_timer < EPSILON) {
        o->sindex++;
        o->move_timer = o->move_period;

        if (o->sindex == 6) {
            o->sindex = 0;
        }

        set_pos_and_size_in_surface(o, o->sindex * 256, o->sy, o->sw, o->sh);
    }

    return true;
}

bool jump (gObject* o) {
    if (o->jump_updates == o->jump_updates_per_step) {
        o->jump_steps--;
        o->jump_updates = 0;
int sign = (o->jump_steps < (o->jump_total_steps / 2)) ? 1 : -1;

    int multiplier = sign;
    if (sign < 0) {
        multiplier = sign * o->jump_steps;
    }
    else {
        multiplier = sign * (o->jump_total_steps - o->jump_steps - 1);
    }

        printf("js %d\n", multiplier);

    
    set_pos_and_size(o, o->x, o->y + 4 * multiplier, o->w, o->h);
    }
    o->jump_updates++;


    if (o->jump_steps == 0) {
        o->jump_steps = o->jump_total_steps;
        o->factions[JUMP] = false;
    }

    return true;
}

bool shoot(gObject* o) { return false; }

gObject* create_game_object(g_object_type t) {
    gObject* ret = malloc(sizeof(gObject));

    switch(t) {
        case PLAYER:
            ret->actions = malloc((LAST_ACTION - 1) * sizeof(pactionf));
            ret->attributes = malloc((LAST_ATTRIBUTE - 1) * sizeof(eAttributes));

            ret->actions[MOVE] = &move;
            ret->actions[JUMP] = &jump;
            ret->actions[SHOOT] = &shoot;

            ret->attributes[CAN_MOVE] = true;
            ret->attributes[CAN_JUMP] = true;
            ret->attributes[CAN_SHOOT] = true;
            ret->attributes[IS_DRAWN] = true;
            ret->move_timer = SPRITE_UPDATE;
            ret->move_period = SPRITE_UPDATE;
            ret->jump_total_steps = 10;
            ret->jump_steps = ret->jump_total_steps;
            ret->jump_updates_per_step = 5;
            ret->jump_updates = 0;
            ret->factions[JUMP] = false;
            ret->factions[SHOOT] = false;
            ret->sindex = 0;
            set_pos_and_size(ret, 640/2 - 128, 480/2 - 128, 256, 256);
            break;
        case ENEMY:
            break;
        case NPC:
            break;
        case ENV:
            break;
        default:
            free(ret);
            ret = NULL;
            break;
    }

    ret->gid = lastid++;

    return ret;
}

void set_pos_and_size(gObject* o, int x, int y, int w, int h) {
    if (o->attributes[IS_DRAWN]) {
        o->x = x;
        o->y = y;
        o->w = w;
        o->h = h;
        o->sx = 0;
        o->sy = 0;
        o->sw = 256;
        o->sh = 256;
    }
} 

void set_pos_and_size_in_surface(gObject* o, int x, int y, int w, int h) {
    if (o->attributes[IS_DRAWN]) {
        o->sx = x;
        o->sy = y;
        o->sw = w;
        o->sh = h;
    }
}

bool load_surface(gObject* o, const char* s) {
    o->surface = IMG_Load(s);
    
    return o->surface == NULL ? false : true;
}

void create_texture(gObject* o, SDL_Renderer* r) {
    o->texture = SDL_CreateTextureFromSurface(r, o->surface);
}

void free_object(gObject* o) {
    SDL_DestroyTexture(o->texture);
    SDL_FreeSurface(o->surface);
}

void step(gObject* o) {
    for(int i = 0; i < LAST_ACTION; i++) {
        if (o->factions[i]) {
            o->actions[i] (o);
        }
    }
}

void set_action(gObject* o, eActions a) {
    o->factions[a] = true;
}

void set_rect(SDL_Rect* r, int x, int y, int w, int h) {
    r->x = x;
    r->y = y;
    r->w = w;
    r->h = h;
}

void object_render_copy(SDL_Renderer* renderer, gObject* o) {
    SDL_Rect srcrect, dstrect;
    //set_rect(&srcrect, index * 256, 0, 256, 256);
    //set_rect(&dstrect, 640/2 - 128, 480/2 - 128 - j, 256, 256);
    set_rect(&srcrect, o->sx, o->sy, o->sw, o->sh);
    set_rect(&dstrect, o->x, o->y, o->w, o->h);

    SDL_RenderCopy(renderer, o->texture, &srcrect, &dstrect);
}
