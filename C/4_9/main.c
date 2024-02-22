#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "BinaryHeap.h"

enum ERRORS
{
    success = 0,
    no_memory = -1,
    runtime_error = -2
};

int main()
{
    Binary_heap_ptr heap = create_binary_heap();
    time_t timer = time(NULL);
    heap -> insert(&heap, 3, timer);
    heap -> insert(&heap, 9, timer);
    heap -> insert(&heap, 1, timer);
    heap -> insert(&heap, 0, timer);
    heap -> insert(&heap, 5, timer);
    heap -> insert(&heap, 4, timer);
    for (int i = 0; i < heap -> size; ++i) printf("%d\n", (heap -> list)[i] -> priority);
    printf("\n_____________\n");
    Binary_heap_ptr second = create_binary_heap();
    second -> insert(&second, 8, timer);
    second -> insert(&second, 6, timer);
    second -> insert(&second, 2, timer);
    second -> insert(&second, 7, timer);
    for (int i = 0; i < second -> size; ++i) printf("%d\n", (second -> list)[i] -> priority);
    printf("\n_____________\n");
    Binary_heap_ptr new = create_binary_heap();
    new -> merge(&new, &heap, &second);
    for (int i = 0; i < new -> size; ++i) printf("%d\n", (new -> list)[i] -> priority);
    printf("\n_____________\n");
    Data_ptr tmp;
    while (tmp = new -> pop(&new))
    {
        printf("min: %d\n", tmp -> priority);
    }
    free_binary_heap(&heap);
    free_binary_heap(&second);
    free_binary_heap(&new);
    return 0;
}