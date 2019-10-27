/*************************PART ONE**************************

This program displays each word/number in the “.obj” file of 
a program in two-byte hexadecimal form
____________________________________________________________

Name: Michael Johnson
Date: 06/05/2019

***********************************************************/

#include <stdio.h>
#include <stdbool.h>

void readFile(FILE *file);
void printBytes(char c1, char c2);

int main(int argc, char **argv) {
    FILE *inputFile; 
    inputFile = fopen(argv[1], "r");
    readFile(inputFile);
    fclose(inputFile);
    return 0;
}

void readFile(FILE *file) {
    if (file == 0) {
        return;
    }
    int hex;
    while ((hex = fgetc(file)) != EOF) {
        printf("0x%02x", hex);
        if ((hex = fgetc(file)) == EOF) {
            break;
        }
        printf("%02x\n", hex);
    }
}

void printBytes(char c1, char c2) {
    printf("0x%02x%02x\n", c1, c2);
}
