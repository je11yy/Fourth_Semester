#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include "Data.h"

typedef struct Node
{
    Data_ptr data;
    int degree;
    struct Node * parent, * child, * sibling;
} Binomial_node, * Binomial_node_ptr;

typedef struct Binomial_heap
{
    Binomial_node_ptr root_list;
    int (*insert) (struct Binomial_heap ** heap, int priority, time_t time);
    Data_ptr (*get_min) (struct Binomial_heap * heap);
    Data_ptr (*pop) (int * error, struct Binomial_heap ** heap);
    int (*merge) (struct Binomial_heap ** new, struct Binomial_heap ** first, struct Binomial_heap ** second);
    int (*meld) (struct Binomial_heap ** new, struct Binomial_heap ** first, struct Binomial_heap ** second);
} Binomial_heap, * Binomial_heap_ptr;

Binomial_node_ptr create_node(int priority, time_t time);
Binomial_heap_ptr create_binomial_heap();
void free_binomial_node(Binomial_node_ptr * node);
int free_binomial_heap(Binomial_heap_ptr * heap);
Data_ptr get_min_binomial(Binomial_heap_ptr heap);
Data_ptr pop_binomial(int * error, Binomial_heap_ptr * heap);
void merge_binomial_trees(Binomial_node_ptr * root);
int insert_binomial(Binomial_heap_ptr * heap, int priority, time_t time);
int add_binomial_heap(Binomial_heap_ptr * heap, Binomial_node_ptr root, int level);
int merge_binomial(Binomial_heap_ptr * new, Binomial_heap_ptr * first, Binomial_heap_ptr * second);
int meld_binomial(Binomial_heap_ptr * new, Binomial_heap_ptr * first, Binomial_heap_ptr * second);
void print_binomial_tree(Binomial_node_ptr root, int level);
void print_binomial_heap(Binomial_heap_ptr heap);
int remove_binomial(Binomial_heap_ptr * heap, Binomial_node_ptr * head, Binomial_node_ptr * bef);

#endif