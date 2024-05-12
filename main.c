/* Trying to make a Pandas clone with C*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define array_length(arr) (sizeof(arr) / sizeof(*(arr)))

// STRUCTS
typedef struct {
    char ***data;
    int rows;
    int cols;
} Table;

typedef struct {
    char **strings;
    int n_strings;
} Str_collection;


// PROTOTYPES
void print_usage(void);
void print_table(Table *table);
Table *read_csv(char *filepath);

char *copy_str(char *str);
int _get_array_lenght_int(int *arr);
int _get_array_lenght_str(char *arr);

Str_collection *_read_file_lines(char *filepath);
void _free_str_collection(Str_collection *coll);
void _print_str_collection(Str_collection *coll);
Str_collection *_split_str(char *arr, char *sep);
char *_join_str(Str_collection *coll, char *sep);

Str_collection *_init_str_collection(char *str);
Str_collection *_init_str_collection(char *str){

    int str_length = strlen(str);
    char *str2 = copy_str(str);

    char **file_lines = malloc(sizeof(char *));
    file_lines[0] = malloc(str_length);
    file_lines[0] = str2;

    Str_collection *coll = (Str_collection *)malloc(sizeof(Str_collection));
    coll->n_strings = 1;
    coll->strings = file_lines;

    return coll;
}

void _append_str_collection(Str_collection *coll,char *str);
void _append_str_collection(Str_collection *coll,char *str){

    int new_index = coll->n_strings;
    char **file_lines = coll->strings;

    int str_length = strlen(str);
    char *str2 = copy_str(str);

    file_lines = realloc(file_lines, sizeof(char *) * (new_index + 1));
    file_lines[new_index] = malloc(str_length);
    file_lines[new_index] = str2;

    coll->n_strings = new_index + 1;
    coll->strings = file_lines;

}



// MAIN
int main(int argc, char *argv[]){

    if (argc == 2)
    {

        Str_collection *raw = _read_file_lines(argv[1]);
        _print_str_collection(raw);

        Str_collection *test = _split_str(raw->strings[1], ",");
        _print_str_collection(test);

        char *stringlet = _join_str(test, "-");
        printf("%s\n", stringlet);

        char *test1 = copy_str(stringlet);
        printf("%s\n", test1);

        Str_collection *init = _init_str_collection("Hello");
        printf("%s\n", init->strings[0]);

        _append_str_collection(init, "Hello2");
        printf("%s\n", init->strings[0]);
        printf("%s\n", init->strings[1]);
        _print_str_collection(init);
        

        //Table *table = read_csv(argv[1]);
        //print_table(table); 
        
        return 0;
    }
    else
    {
        print_usage();
        return 1;
    }
}


// DEFINITIONS
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

Str_collection *_read_file_lines(char *filepath){

    /*
    utility function that will read any text file and
    store its lines content in a dinamically allocated arrays
    of arrays
    */
    
    const int n_lines = 1024;
    const int n_chars = 1024;

    FILE *file_handler = fopen(filepath, "r");
    if (file_handler == NULL) {
        printf("Error opening the file!\n");
        exit(EXIT_FAILURE);
    }
    
    char **file_lines;
    
    // initialize space allocation
    file_lines = malloc(sizeof(char *) * n_lines);
    size_t total_lines_read = 0;
    size_t total_chars_read = 0;
    char read_char;
    
    // loop will read each char from the file until EOF
    do 
    {
        // read char from the file and store it
        read_char = fgetc(file_handler);
        
        // if error occurs reading from file
        if (ferror(file_handler))
        {
            printf("Error reading from file.\n");
            exit(EXIT_FAILURE);
        }
        
        // if we reach the end of the file stop reading from the file
        if (feof(file_handler))
        {

            if (total_chars_read != 0)
            {        
                // allocate enough mem to store the string
                file_lines[total_lines_read] = realloc(file_lines[total_lines_read], total_chars_read + 1 );
                
                // end string by placing null terminator
                file_lines[total_lines_read][total_chars_read] = '\0';
                total_lines_read++;
            }
            
            break; 
        }
        
        if (total_chars_read == 0) 
        {
            file_lines[total_lines_read] = malloc(n_chars);
        }
        
        // store the char into "the current index of the current line" we've read in
        file_lines[total_lines_read][total_chars_read] = read_char; 
        
        // increment total_chars_read as we have just read in another char
        total_chars_read++;
        
        // if the char is a newline, we have reached the end of the current line
        if (read_char == '\n')
        {

            file_lines[total_lines_read] = realloc(file_lines[total_lines_read], total_chars_read + 1 );
            file_lines[total_lines_read][total_chars_read] = '\0';
            
            total_lines_read++;
    
            total_chars_read = 0;
            
            if (total_lines_read % n_lines == 0)
            {
                size_t new_size = total_lines_read + n_lines;
                file_lines = realloc(file_lines, sizeof(char *) * new_size);
            }

        }
        else if (total_chars_read % n_chars == 0)
        {
        // We increase the size of the array by n_chars and use realloc to
        // reallocate space for this larger block of memory.
        size_t new_size = total_chars_read + n_chars;
        file_lines[total_lines_read] = 
            realloc(file_lines[total_lines_read], new_size); 
        }

    } while (true);

    // now we know exactly how many lines we have read from the file and
    // can reallocate the block of memory for the array of pointers to chars to be
    // exactly the size required.
    file_lines = realloc(file_lines, sizeof(char *) * total_lines_read);

    fclose(file_handler);

    // allocate memory for table
    Str_collection *raw = (Str_collection *)malloc(sizeof(Str_collection));
    raw->n_strings = total_lines_read;
    raw->strings = file_lines;

    return raw;
}

void _free_str_collection(Str_collection *coll){

    for (int i = 0; i < coll->n_strings; i++)
        free(coll->strings[i]);
    
    free(coll->strings);
}

void _print_str_collection(Str_collection *coll){

    for (int i = 0; i < coll->n_strings; i++)
        printf("%s ", coll->strings[i]);
    printf("\n");

}

int _get_array_lenght_int(int *arr){
    int length = 0;
    while (arr[length] != '\0') {
        length++;
    }
    return length;
}

int _get_array_lenght_str(char *arr){
    int length = 0;
    while (arr[length] != '\0') {
        length++;
    }
    return length;
}

char *_join_str(Str_collection *coll, char *sep){
    /*
    Takes a an array of strings as input and returns a string
    concatenated by the separator
    */
    const int n_chars_init = 1024;
    int n_chars = 0;

    char* output_arr = malloc(n_chars_init);

    for (int i = 0; i < coll->n_strings; i++){

        char *token = coll->strings[i];
        int str_length = strlen(token);

        for (int j = 0; j <= str_length; j++){

            if ( j == str_length )
                output_arr[n_chars] = sep[0];
            else
                output_arr[n_chars] = token[j];

            n_chars++;
        }
    }

    output_arr = realloc(output_arr, sizeof(char) * (n_chars));
    output_arr[n_chars - 1] = '\0';

    _free_str_collection(coll);

    return output_arr;
}

Str_collection *_split_str(char *arr, char *sep){
    /*
    Takes a string as input and returns 2D array with each
    word that were separated by the sep
    */
    const int n_words = 1024;
    size_t total_words_read = 0;

    char** output_arr = malloc(sizeof(char *) * n_words);

    char* token = strtok(arr, sep);
    while (token != NULL) {

        int token_lenght = strlen(token);

        output_arr[total_words_read] = malloc(token_lenght);
        strcpy(output_arr[total_words_read], token);
        token = strtok(NULL, ",");

        total_words_read++;
    }

    output_arr = realloc(output_arr, sizeof(char *) * total_words_read);

    // allocate memory for output struct
    Str_collection *raw = (Str_collection *)malloc(sizeof(Str_collection));
    raw->n_strings = total_words_read;
    raw->strings = output_arr;

    return raw;
}

char *copy_str(char *str){
    /*
    Put array string on the heap 
    */

    char *output = malloc(strlen(str) * sizeof(char));
    strcpy(output, str);
    //free(str);

    return output;
}