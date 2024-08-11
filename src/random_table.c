#include "random_table.h"
#include "raylib.h"



random_table* random_table_ctor(int size, int min, int max) {
    random_table* table = malloc(sizeof(*table) + sizeof(int) * size);
    
    if (table == NULL) {
        fprintf(stderr, "Failed to create table");
        abort();
    }

    table->length = size;
    table->index = 0;
    SetRandomSeed(510912122);

    for (int i = 0; i < size; i++) {
        table->values[i] = GetRandomValue(min, max);
    }
    return table;
}

int next_value(random_table* table)
{
    int value = table->values[table->index];
    table->index++;
    if (table->index >= table->length) {
        table->index -= table->length;
    }
    return value;
}
