/*
Dinamic arrays to work with strings

*/
typedef struct {
    char **strings;
    int n_strings;
} Str_collection;
Str_collection *_init_str_collection(char *str);
Str_collection *_read_file_lines(char *filepath);
Str_collection *_split_str(char *arr, char *sep);
char *_join_str(Str_collection *coll, char *sep);
void _append_str_collection(Str_collection *coll,char *str);
void _free_str_collection(Str_collection *coll);
void _print_str_collection(Str_collection *coll);
char *copy_str(char *str);
int _get_array_lenght_str(char *arr);