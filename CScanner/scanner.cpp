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
    // while (1) {
    //     fp = scan(fp);
    //     if (feof(fp)) break;
    // }
    scan(fp);
    fclose(fp);
    fp = NULL;
}