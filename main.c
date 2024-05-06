/* Trying to make a Pandas clone with C*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// DEFINITIONS
void usage(void);
void print_table(char table[10][10][100]);


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
            char buffer[100];
            char table[10][10][100];

            int i = 0; //row index
            int j = 0; //col index
            
            // iterate over all the rows of the file
            while (fgets (buffer, 100, file_handler) != NULL)
            {   
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
        usage();
        return 1;
    }
}


// IMPLEMENTATION
void usage(void){
    printf("\n1 argument expected\n");
    printf("arg1: provide path to file\n");
    printf("\n");
}

// TODO: FIX THIS
void print_table(char table[10][10][100]){
    int i, j, k;

    // Print column headers
    printf("      ");
    for (j = 0; j < 10; j++) {
        printf("| %-10d", j);
    }
    printf("\n");

    // Print separator line
    for (j = 0; j <= 10; j++) {
        printf("+------------");
    }
    printf("+\n");

    // Print array contents
    for (i = 0; i < 10; i++) {
        printf("%-5d ", i); // Print row index

        for (j = 0; j < 10; j++) {
            printf("| %-10s", table[i][j]); // Print array element
        }
        printf("\n");
    }
}
