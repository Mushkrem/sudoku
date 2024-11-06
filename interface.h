#pragma once

typedef struct _label {
    unsigned char bit;
    int weight;
    char* text;
} _label;

typedef struct UINTERFACE {
	int (*read_resources)();
} UINTERFACE;

UINTERFACE get_interface();