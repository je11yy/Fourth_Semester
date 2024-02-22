#include "BinaryHeap.h"

enum ERRORS
{
    success = 0,
    no_memory = -1,
    runtime_error = -2
};

void free_binary_heap(Binary_heap_ptr * heap)
{
    if (!(*heap)) return;
    if ((*heap) -> list)
    {
        for (int i = 0; i < (*heap) -> size; ++i)
        {
            free(((*heap) -> list)[i]);
            ((*heap) -> list)[i] = NULL;
            
        }
        free((*heap) -> list);
    }
    free(*heap);
}

Binary_heap_ptr create_binary_heap()
{
    Binary_heap_ptr heap = (Binary_heap_ptr)malloc(sizeof(Binary_heap));
    if (!heap) return NULL;
    Data_array list = (Data_array)malloc(sizeof(Data_ptr));
    if (!list)
    {
        free(heap);
        return NULL;
    }
    heap -> list = list;
    heap -> capacity = 1;
    heap -> size = 0;
    heap -> insert = insert_binary;
    heap -> get_min = get_min_binary;
    heap -> pop = pop_binary;
    heap -> merge = merge_binary;
    heap -> meld = meld_binary;
    return heap;
}

int insert_binary(Binary_heap_ptr * heap, int priority, time_t time)
{
    Data_ptr data = create_data(priority, time);
    if (!data)
    {
        free_binary_heap(heap);
        return no_memory;
    }
    if (!(*heap))
    {
        *heap = create_binary_heap();
        if (!(*heap))
        {
            free(data);
            return no_memory;
        }
    }
    if ((*heap) -> size + 1 > (*heap) -> capacity)
    {
        (*heap) -> capacity *= 2;
        Data_array tmp = (Data_array)realloc((*heap) -> list, ((*heap) -> capacity) * sizeof(Data_ptr));
        if (!tmp)
        {
            free(data);
            free_binary_heap(heap);
            return no_memory;
        }
        (*heap) -> list = tmp;
    }
    ((*heap) -> list)[(*heap) -> size] = data;
    sift_up_binary(heap, (*heap) -> size);
    ((*heap) -> size)++;
    return success;
}

void sift_down_binary(Binary_heap_ptr * heap, int index)
{
    while (2 * index + 1 < (*heap) -> size)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int j = left;
        if (right < (*heap) -> size && cmp_data(((*heap) -> list)[right], ((*heap) -> list)[left]) < 0) j = right;
        if (cmp_data(((*heap) -> list)[index], ((*heap) -> list)[j]) <= 0) break;
        swap(&(((*heap) -> list)[index]), &(((*heap) -> list)[j]));
        index = j;
    }
}

void sift_up_binary(Binary_heap_ptr * heap, int index)
{
    while (index > 0 && cmp_data(((*heap) -> list)[index], ((*heap) -> list)[(index - 1) / 2]) < 0)
    {
        swap(&(((*heap) -> list)[index]), &(((*heap) -> list)[(index - 1) / 2]));
        index = (index - 1) / 2;
    }
}

Data_ptr get_min_binary(Binary_heap_ptr heap)
{
    if (!heap) return NULL;
    if (heap -> size == 0) return NULL;
    return (heap -> list)[0];
}

Data_ptr pop_binary(Binary_heap_ptr * heap)
{
    if (!(*heap)) return NULL;
    if ((*heap) -> size == 0) return NULL;
    Data_ptr tmp = ((*heap) -> list)[0];
    ((*heap) -> list)[0] = ((*heap) -> list)[(*heap) -> size - 1];
    ((*heap) -> size)--;
    sift_down_binary(heap, 0);
    return tmp;
}

/*
слияние без разрушения
*/
int merge_binary(Binary_heap_ptr * new, Binary_heap_ptr * first, Binary_heap_ptr * second)
{
    if (*new) free_binary_heap(new);
    Binary_heap_ptr heap = create_binary_heap();
    if (!heap)
    {
        free_binary_heap(first);
        free_binary_heap(second);
        return no_memory;
    }
    for (int i = 0; i < (*first) -> size; ++i)
    {
        if (heap -> insert(&heap, ((*first) -> list)[i] -> priority, ((*first) -> list)[i] -> time) < 0)
        {
            free_binary_heap(first);
            free_binary_heap(second);
            return no_memory;
        }
    }
    for (int i = 0; i < (*second) -> size; ++i)
    {
        if (heap -> insert(&heap, ((*second) -> list)[i] -> priority, ((*second) -> list)[i] -> time) < 0)
        {
            free_binary_heap(first);
            free_binary_heap(second);
            return no_memory;
        }
    }
    *new = heap;
    return success;
}

/*
слияние с разрушением
*/
int meld_binary(Binary_heap_ptr * new, Binary_heap_ptr * first, Binary_heap_ptr * second)
{
    if (*new) free_binary_heap(new);
    Binary_heap_ptr heap = create_binary_heap();
    if (!heap)
    {
        free_binary_heap(first);
        free_binary_heap(second);
        return no_memory;
    }
    Data_ptr tmp = NULL;
    int error;
    while (tmp = ((*first) -> pop(first)))
    {
        error = heap -> insert(&heap, tmp -> priority, tmp -> time);
        free(tmp);
        if (error < 0)
        {
            free_binary_heap(first);
            free_binary_heap(second);
            return no_memory;
        }
    }
    while (tmp = ((*second) -> pop(second)))
    {
        error = heap -> insert(&heap, tmp -> priority, tmp -> time);
        free(tmp);
        if (error < 0)
        {
            free_binary_heap(first);
            free_binary_heap(second);
            return no_memory;
        }
    }
    free_binary_heap(first);
    free_binary_heap(second);
    *new = heap;
    return success;
}