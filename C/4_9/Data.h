#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Data
{
    int priority;
    time_t time;
} Data, * Data_ptr, ** Data_array;

int cmp_data(Data_ptr first, Data_ptr second);
Data_ptr create_data(int priority, time_t time);
void swap(Data_ptr * first, Data_ptr * second);

#endif