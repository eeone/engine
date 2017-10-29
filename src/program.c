#include "program.h"
#include "gobject.h"
#include "gtime.h"

double time_since_start;
const double ccdt = SPRITE_UPDATE;
const double ppdt = CHECKPOINT;
double pdt = CHECKPOINT;
double for_real_time_since_start;
double for_real_simulated = 0;

void input(unsigned char *quit, gObject* o) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)

    {
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_w) {
                set_action(o, JUMP);
            }
            break;
        case SDL_QUIT:
            *quit = true;
            break;
    }
}

void update(gObject* o) {
    step(o);

    pdt -= UPDATE_STEP;
    for_real_simulated += UPDATE_STEP;

    if (pdt <= EPSILON) {
        pdt = ppdt;
        double t = get_time();
        printf("Real: %f Jitter: %fus Total: %f\n", 
                t - time_since_start, 
                MILLION * (t - time_since_start - ppdt), 
                get_time() - for_real_time_since_start - for_real_simulated);
        time_since_start = t;
    }

}

void render(SDL_Renderer* renderer, gObject* o) {
    SDL_RenderClear(renderer); 
    object_render_copy(renderer, o);
    SDL_RenderPresent(renderer);
}

int main (int argc, char** argv) {
    printf("Loading.\n");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL.\n");
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Unable to initialize SDL image.\n");
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Sprite Sheets",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    gObject* player = create_game_object(PLAYER);
    if (!load_surface(player, "assets/spritestrip.png")) {
        return -1;
    }
    create_texture(player, renderer);
    set_action(player, MOVE);

    printf("Starting.\n");

    time_since_start = get_time();
    for_real_time_since_start = get_time();
    const double dt = UPDATE_STEP;
    double accumulator = 0;

    double previous_time = get_time();

    unsigned char quit = false;
    while (!quit) {
        double current_time = get_time();
        double frame_time = current_time - previous_time;
        previous_time = current_time;

        if (frame_time >= FRAME_SKIP) {
            printf("dropped, %f!\n", frame_time);
            frame_time = FRAME_SKIP;
        }

        accumulator += frame_time;
        while (accumulator >= dt) {
            accumulator -= dt;
            update(player);
        }

        render(renderer, player);

        input(&quit, player);
   }

    free_object(player);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
