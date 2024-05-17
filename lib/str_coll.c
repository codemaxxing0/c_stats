#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char **strings;
    int n_strings;
} Str_collection;

char *copy_str(char *str){
    /*
    Put array string on the heap 
    */

    char *output = malloc(strlen(str) * sizeof(char));
    strcpy(output, str);
    //free(str);

    return output;
}

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
