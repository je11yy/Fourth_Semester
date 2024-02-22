#include "Data.h"

typedef struct Binary_heap
{
    Data_array list;
    int capacity;
    int size;

    int (*insert) (struct Binary_heap ** heap, int priority, time_t time);
    Data_ptr (*get_min) (struct Binary_heap * heap);
    Data_ptr (*pop) (struct Binary_heap ** heap);
    int (*merge) (struct Binary_heap ** new, struct Binary_heap ** first, struct Binary_heap ** second);
    int (*meld) (struct Binary_heap ** new, struct Binary_heap ** first, struct Binary_heap ** second);
} Binary_heap, * Binary_heap_ptr;

void free_binary_heap(Binary_heap_ptr * heap);
Binary_heap_ptr create_binary_heap();
int insert_binary(Binary_heap_ptr * heap, int priority, time_t time);
void sift_down_binary(Binary_heap_ptr * heap, int index);
void sift_up_binary(Binary_heap_ptr * heap, int index);
Data_ptr get_min_binary(Binary_heap_ptr heap);
Data_ptr pop_binary(Binary_heap_ptr * heap);
int merge_binary(Binary_heap_ptr * new, Binary_heap_ptr * first, Binary_heap_ptr * second);
int meld_binary(Binary_heap_ptr * new, Binary_heap_ptr * first, Binary_heap_ptr * second);