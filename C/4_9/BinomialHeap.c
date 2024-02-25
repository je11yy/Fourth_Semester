#include "BinomialHeap.h"

enum ERRORS
{
    success = 0,
    no_memory = -1,
    runtime_error = -2,
    not_found = -3,
    empty = -4
};

void print_binomial_tree(Binomial_node_ptr root, int level)
{
    if (!root) return;
    for (int i = 0; i < level; ++i) printf("   ");
    printf("%d\n", root -> data -> priority);
    Binomial_node_ptr tmp = root -> child;
    while (tmp)
    {
        print_binomial_tree(tmp, level + 1);
        tmp = tmp -> sibling;
    }
}

void print_binomial_heap(Binomial_heap_ptr heap)
{
    Binomial_node_ptr current = heap -> root_list;
    while (current)
    {
        print_binomial_tree(current, 0);
        current = current -> sibling;
    }
}

Binomial_node_ptr create_node(int priority, time_t time)
{
    Data_ptr data = create_data(priority, time);
    if (!data) return NULL;
    Binomial_node_ptr node = (Binomial_node_ptr)malloc(sizeof(Binomial_node));
    if (!node)
    {
        free(data);
        data = NULL;
        return NULL;
    }
    node -> data = data;
    node -> degree = 0;
    node -> parent = node -> child = node -> sibling = NULL;
    return node;
}

Binomial_heap_ptr create_binomial_heap()
{
    Binomial_heap_ptr heap = (Binomial_heap_ptr)malloc(sizeof(Binomial_heap));
    if (!heap) return NULL;
    heap -> root_list = NULL;
    heap -> insert = insert_binomial;
    heap -> get_min = get_min_binomial;
    heap -> pop = pop_binomial;
    heap -> merge = merge_binomial;
    heap -> meld = meld_binomial;
    return heap;
}

void free_binomial_node(Binomial_node_ptr * node)
{
    free((*node) -> data);
    (*node) -> data = NULL;
    (*node) -> parent = NULL;
    (*node) -> child = NULL;
    (*node) -> sibling = NULL;
    free(*node);
    *node = NULL;
}

int free_binomial_heap(Binomial_heap_ptr * heap)
{
    if (!(*heap)) return success;
    if (!((*heap) -> root_list))
    {
        free(*heap);
        *heap = NULL;
        return success;
    }
    Data_ptr data;
    int error = 0;
    while (error != not_found)
    {
        data = pop_binomial(&error, heap);
        free(data);
        data = NULL;
        if (error == no_memory) return no_memory;
    }
    free(*heap);
    *heap = NULL;
}

Data_ptr get_min_binomial(Binomial_heap_ptr heap)
{
    if (!heap) return NULL;
    Binomial_node_ptr tmp = heap -> root_list;
    if (!tmp) return NULL;
    
    Binomial_node_ptr min = heap -> root_list;
    Binomial_node_ptr next = min -> sibling;
    while (next)
    {
        if (cmp_data(next -> data, min -> data) < 0) min = next;
        next = next -> sibling;
    }
    Data_ptr data = min -> data;

    return data;
}

int remove_binomial(Binomial_heap_ptr * heap, Binomial_node_ptr * head, Binomial_node_ptr * bef)
{
    if ((*head) == (*heap) -> root_list) (*heap) -> root_list = (*head) -> sibling;
    else (*bef) -> sibling = (*head) -> sibling;

    Binomial_node_ptr new_head = (*head) -> child;

    Binomial_heap_ptr new = create_binomial_heap();
    if (!new)
    {
        free_binomial_heap(heap);
        free_binomial_node(head);
        free_binomial_node(bef);
        return no_memory;
    }
    new -> root_list = new_head;
    Binomial_heap_ptr tmp = NULL;
    int error = meld_binomial(&tmp, heap, &new);
    if (error < 0)
    {
        free_binomial_node(head);
        free_binomial_node(bef);
        return no_memory;
    }
    *heap = tmp;
    return success;
}

Data_ptr pop_binomial(int * error, Binomial_heap_ptr * heap)
{
    if (!(*heap))
    {
        *error = not_found;
        return NULL;
    }
    if (!((*heap) -> root_list))
    {
        *error = not_found;
        return NULL;
    }
    Binomial_node_ptr min = (*heap) -> root_list;
    Binomial_node_ptr min_prev = NULL;
    Binomial_node_ptr next = min -> sibling;
    Binomial_node_ptr next_prev = min;
    while (next)
    {
        if (cmp_data(next -> data, min -> data) < 0)
        {
            min = next;
            min_prev = next_prev;
        }
        next_prev = next;
        next = next -> sibling;
    }
    Data_ptr data = min -> data;
    
    if (remove_binomial(heap, &min, &min_prev) < 0)
    {
        *error = no_memory;
        return NULL;
    }

    min -> parent = min -> child = min -> sibling = NULL;
    free(min);
    min = NULL;
    *error = success;
    return data;
}

void merge_binomial_trees(Binomial_node_ptr * root)
{
    Binomial_node_ptr current = *root;
    Binomial_node_ptr prev = NULL;
    Binomial_node_ptr tmp;
    int cmp_res;
    while (current -> sibling)
    {
        if (current -> degree == current -> sibling -> degree)
        {
            cmp_res = cmp_data(current -> data, current -> sibling -> data);
            if (cmp_res > 0)
            {
                if (prev) prev -> sibling = current -> sibling;
                else *root = current -> sibling;
                tmp = current -> sibling;
                tmp -> degree += 1;
                current -> sibling = current -> sibling -> child;
                tmp -> child = current;
                current = tmp;
            }
            else
            {
                tmp = current -> sibling;
                current -> sibling = tmp -> sibling;
                tmp -> sibling = current -> child;
                current -> child = tmp;
            }
        }
        else
        {
            prev = current;
            current = current -> sibling;
        }
    }
}

int insert_binomial(Binomial_heap_ptr * heap, int priority, time_t time)
{
    Binomial_node_ptr node = create_node(priority, time);
    if (!node)
    {
        free_binomial_heap(heap);
        return no_memory;
    }
    Binomial_heap_ptr new = create_binomial_heap();
    if (!heap)
    {
        free_binomial_node(&node);
        free_binomial_heap(heap);
        return no_memory;
    }
    new -> root_list = node;
    Binomial_heap_ptr newest = NULL;
    new -> meld(&newest, heap, &new);
    *heap = newest;
    return success;
}

int add_binomial_heap(Binomial_heap_ptr * heap, Binomial_node_ptr root, int level)
{
    if (!(*heap)) return no_memory;
    if (!root) return success;
    int error;
    if (root -> child) error = add_binomial_heap(heap, root -> child, level + 1);
    if (level) if (root -> sibling) error = add_binomial_heap(heap, root -> sibling, level);
    error = (*heap) -> insert(heap, root -> data -> priority, root -> data -> time);
    if (error < 0)
    {
        free_binomial_heap(heap);
        return no_memory;
    }
}

int merge_binomial(Binomial_heap_ptr * new, Binomial_heap_ptr * first, Binomial_heap_ptr * second)
{
    if (*new) free_binomial_heap(new);
    *new = create_binomial_heap();
    if (!(*new))
    {
        free_binomial_heap(first);
        free_binomial_heap(second);
        return no_memory;
    }
    if ((*first) -> root_list == NULL)
    {
        (*new) -> root_list = (*second) -> root_list;
        return success;
    }
    if ((*second) -> root_list == NULL)
    {
        (*new) -> root_list = (*first) -> root_list;
        return success;
    }

    Binomial_node_ptr head;
    Binomial_node_ptr first_node = (*first) -> root_list;
    Binomial_node_ptr second_node = (*second) -> root_list;
    Binomial_node_ptr tail;

    if (first_node -> degree <= second_node -> degree)
    {
        head = first_node;
        first_node = first_node -> sibling;
    }
    else
    {
        head = second_node;
        second_node = second_node -> sibling;
    }
    tail = head;
    while (first_node && second_node)
    {
        if (first_node -> degree <= second_node -> degree)
        {
            tail -> sibling = first_node;
            first_node = first_node -> sibling;
        }
        else
        {
            tail -> sibling = second_node;
            second_node = second_node -> sibling;
        }
        tail = tail -> sibling;
    }
    tail -> sibling = (first_node) ? first_node : second_node;
    merge_binomial_trees(&head);
    (*new) -> root_list = head;
    return success;
}

int meld_binomial(Binomial_heap_ptr * new, Binomial_heap_ptr * first, Binomial_heap_ptr * second)
{
    int error = merge_binomial(new, first, second);
    if (error < 0) return error;
    free(*first);
    *first = NULL;
    free(*second);
    *second = NULL;
    return success;
}