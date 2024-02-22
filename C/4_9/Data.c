#include "Data.h"

Data_ptr create_data(int priority, time_t time)
{
    Data_ptr data = (Data_ptr)malloc(sizeof(Data));
    if (!data) return NULL;
    data -> priority = priority;
    data -> time = time;
    return data;
}

void swap(Data_ptr * first, Data_ptr * second)
{
    Data_ptr tmp = *first;
    *first = *second;
    *second = tmp;
}

int cmp_data(Data_ptr first, Data_ptr second)
{
    int cmp_priority = first -> priority - second -> priority;
    if (cmp_priority != 0) return cmp_priority;
    return difftime(first -> time, second -> time);
}
