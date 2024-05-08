/* Trying to make a Pandas clone with C*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// TABLE STRUCT
typedef struct {
    char ***data;
    int rows;
    int cols;
} Table;

// PROTOTYPES
void print_usage(void);
void print_table(Table *table);
Table *read_csv(char *filepath);


// MAIN
int main(int argc, char *argv[]){

    if (argc == 2)
    {

        Table *table = read_csv(argv[1]);
        print_table(table); 
        
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

void print_table(Table *table) {

    for (int i = 0; i < table->rows; i++) {
        printf("%-5d ", i);

        for (int j = 0; j < table->cols; j++) {
            printf("| %-10s", table->data[i][j]);
        }
        printf("\n");
    }
}

// TODO: separate in smaller function
// TODO: add some more free()
Table *read_csv(char *filepath) {
    FILE *file_handler = fopen(filepath, "r");
    if (file_handler == NULL) {
        printf("Error opening the file!\n");
        return NULL;
    }

    // TODO: find a better way to do this
    // first scan the file to find these 4 values to 
    // allocate needed memory 
    int line_count = 0;
    // calculate this allocate a big enough buffer to store the longest cell
    size_t max_line_length = 0;
    size_t line_length;

    // calculate allocation parameters
    char *line = NULL;
    size_t buffer_size = 0;
    while (getline(&line, &buffer_size, file_handler) != -1) {
        line_count++;
        line_length = strlen(line);
        if (line_length > max_line_length) {
            max_line_length = line_length;
        }
    }
    free(line);  // deallocate last line buffer

    // reset file position to start of file
    fseek(file_handler, 0, SEEK_SET);

    // allocate memory for table
    Table *table = (Table *)malloc(sizeof(Table));
    table->rows = line_count;
    table->cols = 0;
    table->data = (char ***)malloc(line_count * sizeof(char **));
    if (table->data == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file_handler);
        return NULL;
    }

    // allocate buffer based on the longest cell
    char *buffer = (char *)malloc((max_line_length + 1) * sizeof(char));
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file_handler);
        return NULL;
    }

    // reading file again to populate table (this sucks)
    int i = 0;
    while (fgets(buffer, max_line_length + 1, file_handler) != NULL) {
        char *token;
        int col_count = 0;
        token = strtok(buffer, ",");
        while (token != NULL) {
            col_count++;
            token = strtok(NULL, ",");
        }
        if (col_count > table->cols) {
            table->cols = col_count;
        }
        i++;
    }

    // allocate memory for data cells
    for (int i = 0; i < line_count; i++) {
        table->data[i] = (char **)malloc(table->cols * sizeof(char *));
        if (table->data[i] == NULL) {
            printf("Memory allocation failed!\n");
            fclose(file_handler);
            return NULL;
        }
        for (int j = 0; j < table->cols; j++) {
            table->data[i][j] = (char *)malloc(100 * sizeof(char));
            if (table->data[i][j] == NULL) {
                printf("Memory allocation failed!\n");
                return NULL;
            }
        }
    }

    // reset file position to start of file
    fseek(file_handler, 0, SEEK_SET);

    // populate table
    i = 0;
    while (fgets(buffer, max_line_length + 1, file_handler) != NULL) {
        char *token;
        int j = 0;
        token = strtok(buffer, ",");
        while (token != NULL && j < table->cols) {
            strcpy(table->data[i][j], token);
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }

    fclose(file_handler);
    free(buffer);
    return table;
}