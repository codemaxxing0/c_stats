#include <stdio.h>
#include <string.h>
#include "cell.h"

// Memory allocator for cell
Cell *newCell(void){

    Cell *cell = malloc(sizeof (Cell));
    if (cell == NULL){
        return NULL;
    }

    return cell;

}

// Set cell content values
void setCellItems(Cell *cell, int idx, int idy, char *data){
    cell->data = data;
    cell->idx = idx;
    cell->idy = idy;
}

// Memory deallocator for cell
void delCell(Cell *cell) {

    if (cell != NULL) {
        free(cell->data);
        free(cell);
    }
}

