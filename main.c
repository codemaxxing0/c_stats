/* Trying to make a Pandas clone with C*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N_ROWS 5
#define N_COLS 3
#define N_CHARS 100

// DEFINITIONS
void print_usage(void);
void print_table(char table[N_ROWS][N_COLS][N_CHARS]);


// MAIN
int main(int argc, char *argv[]){

    if (argc == 2)
    {

        FILE *file_handler;
        file_handler = fopen(argv[1], "r");

        if (file_handler == NULL)
        {
            printf("Error opening the file!\n");
            return 1;
        }
        else
        {
            // TODO: make dinamically allocated
            char buffer[N_CHARS];
            char table[N_ROWS][N_COLS][N_CHARS];

            int i = 0; //row index
            
            // iterate over all the rows of the file
            while (fgets (buffer, N_CHARS, file_handler) != NULL)
            {   
                int j = 0; //col index
                
                // iterate over all the tokens divided by sep
                // TODO: make separator a parameter
                // TODO: handle commas inside the cell
                char *token = strtok(buffer, ",");
                while (token != NULL)
                {   
                    // assing the cell value by copying the token
                    strcpy(table[i][j], token);
                    // break from the while loop
                    token = strtok (NULL, ",");
                    j++;
                }
                
                i++;
            }
            fclose(file_handler);

            print_table(table);
        }
        
        return 0;
    }
    else
    {
        print_usage();
        return 1;
    }
}


// IMPLEMENTATION
void print_usage(void){
    printf("\n1 argument expected\n");
    printf("arg1: provide path to file\n");
    printf("\n");
}

// TODO: make dinamic
void print_table(char table[N_ROWS][N_COLS][N_CHARS]){
    int i, j;

    for (i = 0; i < N_ROWS; i++) {
        printf("%-5d ", i);

        for (j = 0; j < N_COLS; j++) {
            printf("| %-10s", table[i][j]);
        }
        printf("\n");
    }
}