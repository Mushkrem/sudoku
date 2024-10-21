#include <stdio.h>
#include <stdlib.h>

typedef struct _global {
    int points;
    int position;
} _global;

typedef struct _menu {
    int position;
    char* labels[];
} _menu;

int main(int argc, char* argv[]) {    
    typedef struct _game {
        int points;
        _global* global_ref;
    } _game;

    _global global;
    _game game;

    global.position = 1;
    game.global_ref = &global;

    printf("Hello world!\n %d, %s", game.global_ref->position, " :)");
}