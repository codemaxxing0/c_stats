#include <stdio.h>
#include <string.h>
#include "cell.h"

char table[5][3] = {0.0};

int main(void){

    char filepath[100] = "/home/matt/Desktop/c_stats/examples/example_01.csv";
    FILE* contents = fopen(filepath, "r");
    rewind(contents);

    int idx = -1;
    int idy = -1;

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), contents) != NULL){

        ++idx;
        printf("%s\n", buffer);
        char *token = strtok(buffer, ",");

        while(token){
            ++idy;
            token = strtok(NULL, ",");
            puts(token);
            
        }
        
    }
    

    return 0;

}