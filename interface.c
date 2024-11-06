#pragma once

#include "interface.h"

//int read_labels_to(_label** labels) {
//    FILE* stream;
//    char text[81], bit[5], sub_text[81];
//    int i = 0;
//
//    errno_t error = fopen_s(&stream, "en_uk.txt", "r");
//    if (error)
//        return EXIT_FAILURE;
//    
//    // Scanning the file assumming correct syntax.
//    while (fscanf_s(stream, "%80[^:]:%4s %[^\n]\n", text, (unsigned)_countof(text), bit, (unsigned)_countof(bit), sub_text, (unsigned)_countof(sub_text)) != EOF) {
//        labels[i] = (_label*)malloc(sizeof(_label));
//        if (labels[i] == NULL)
//            return EXIT_FAILURE;
//        
//        // Null-terminating strings.
//        text[80] = '\0';
//        sub_text[80] = '\0';
//        bit[4] = '\0';
//
//        labels[i]->bit = atoi(bit);
//        labels[i]->weight = i;
//        labels[i]->text = (char*)malloc(81 * sizeof(char));
//        if (labels[i]->text == NULL)
//            return EXIT_FAILURE;
//
//        // Parsing sub labels.
//        labels[i]->sub_count = 0;
//        labels[i]->sub_text = (char*)malloc(81 * sizeof(char));
//        labels[i]->sub_text[0] = "Hello";
//        labels[i]->sub_text[0] = "World!";
//
//        
//        labels[i]->sub_text = (char*)malloc(81 * sizeof(char));
//        if (labels[i]->sub_text == NULL)
//            return EXIT_FAILURE;
//
//        snprintf(labels[i]->text, 81, "%s", text);
//
//        i++;
//    }
//
//    fclose(stream);
//    return EXIT_SUCCESS;
//}

int read_resources_impl(_label** label) {
	return 1;
}

UINTERFACE get_interface() {
	UINTERFACE ui;

	ui.read_resources = read_resources_impl;

	return ui;
}