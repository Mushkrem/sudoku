#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "screen.h"
#include "input.h"

typedef struct _global {
    int points;
    int position;
} _global;

typedef struct _label {
    unsigned char bit;
    int weight;
    char* text;
} _label;

typedef struct _menu {
    unsigned char position;
    _label** labels;
} _menu;


int read_labels_to(_label** labels) {
    FILE* stream;
    char text[81], bit[5];
    int i = 0;

    errno_t error = fopen_s(&stream, "labels.txt", "r");
    if (error)
        return EXIT_FAILURE;
    
    while (fscanf_s(stream, "%80[^:]:%4s\n", text, (unsigned)_countof(text), bit, (unsigned)_countof(bit)) != EOF) {
        labels[i] = (_label*)malloc(sizeof(_label));
        if (labels[i] == NULL)
            return EXIT_FAILURE;
        
        text[80] = '\0';
        bit[4] = '\0';

        printf("%d ", atoi(bit));

        labels[i]->bit = atoi(bit);
        labels[i]->weight = i;
        labels[i]->text = (char*)malloc(81 * sizeof(char));
        if (labels[i]->text == NULL)
            return EXIT_FAILURE;
        snprintf(labels[i]->text, 81, "%s", text);

        i++;
    }

    fclose(stream);
    return EXIT_SUCCESS;
}

typedef struct _game {
    int points;
    _global* global_ref;
} _game;

int main(int argc, char* argv[]) {
    HANDLE thread;
    DWORD thread_id;
    int thread_number = 1;

    INPUTS input = get_inputs();
    
    _global global;
    _game game;

    global.position = 1;
    game.global_ref = &global;

    _label label;
    _menu menu;

    menu.labels = (_label**)malloc(8 * sizeof(_label));

    read_labels_to(menu.labels);

    for (int i = 0; i < 4; i++) {
        printf("Label %d: bit=%d, weight=%d, text=%s\n", i, menu.labels[i]->bit, menu.labels[i]->weight, menu.labels[i]->text);
    }

    thread = CreateThread(NULL, 0, input.listen, &thread_number, 0, &thread_id);

    if (thread == NULL) {
        fprintf(stderr, "Error creating a new thread.\n");
        return EXIT_FAILURE;
    }

    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);


    return EXIT_SUCCESS;
}