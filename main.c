#include <stdio.h>

int main(void){

    char filepath[100] = "/home/matt/Desktop/c_pandas/examples/example_01.csv";
    
    FILE* contents = fopen(filepath, "r");

    rewind(contents);

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), contents) != NULL){
        printf("%s\n", buffer);
    }
    

    return 0;

}