#include "file.h"

#include <stdio.h>
#include <stdlib.h>

int file_read_all(const char* path, char** buffer, int* len) {
    *buffer = NULL;
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        return 1;
    }
    fseek(fp, 0l, SEEK_END);
    long size = ftell(fp);
    *buffer = malloc(sizeof(char) * (size + 1));
    fseek(fp, 0l, SEEK_SET);
    size = fread(*buffer, sizeof(char), size, fp);
    fclose(fp);
    (*buffer)[size] = '\0';
    if (len) {
        *len = size;
    }
    return 0;
}