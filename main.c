#include <stdio.h>
#include <string.h>
#include "cell.h"

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
            
            Cell table_cell;
            setCellItems(&table_cell, idx, idy, *token);
            //puts(token);

            token = strtok(NULL, ",");
        }
        
    }
    

    return 0;

}