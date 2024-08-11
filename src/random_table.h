#include <stdio.h>
#include <stdlib.h>

typedef struct random_table random_table;

struct random_table {
    int index;
    int length;
    int values[];

};

random_table* random_table_ctor(int size, int min, int max);
int next_value(random_table* table);