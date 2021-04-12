#include <iostream>
#include <stdio.h>
#include "token.h"
using namespace std;

int main(int argc, char* argv[]) {
    FILE* fp;
    if (argc > 1) {
        if ((fp = fopen(argv[1], "r")) == NULL) {
            printf("Unable to open file.");
            exit(0);
        }
    }
    printf("%s", "Result: \n");
    scan(fp);
    fclose(fp);
    fp = NULL;
}