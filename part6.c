/*************************PART SIX**************************
 
Name: Michael Johnson
Date: 07/05/2019

***********************************************************/

#include "lc3.c"

int main(int argc, char **argv) {
    if (argv[1] == NULL) {
        printf("Please provide an .obj file parameter\n");
    } else {
        FILE *inputFile; 
        inputFile = fopen(argv[1], "r");
        loadProgram(inputFile, NOT);
    }
}
