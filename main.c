#include <stdio.h>
#include <string.h>

int main(void){

    char filepath[100] = "/home/matt/Desktop/c_stats/examples/example_01.csv";
    
    FILE* contents = fopen(filepath, "r");

    rewind(contents);

    typedef struct
    {
        int idx;
        int idy;
        char value[100];
    } Cell;
    
    int idx = -1;
    int idy = -1;

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), contents) != NULL){

        ++idx;
        printf("%s\n", buffer);
        char *token = strtok(buffer, ",");

        while(token){
            ++idy;
            puts(token);
            token = strtok(NULL, ",");
        }
        
    }
    

    return 0;

}