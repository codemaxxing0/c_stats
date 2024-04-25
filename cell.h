typedef struct{
    int idx;
    int idy;
    char *data;
} Cell;

Cell *newCell(void);
void delCell(Cell *cell);
void setCellItems(Cell *cell, int idx, int idy, char *data);