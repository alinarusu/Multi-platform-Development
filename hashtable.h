#ifndef __HAHSTABLE__
#define __HAHSTABLE__

typedef struct Node {
    char *str;          // actual words, values
    struct Node *succ;  // pointer to the next node in list
} node;

typedef struct Htable {
    node **v;           // buckets vector
    int dim;            // hash table size
} htable;

void alloc(htable **h, int dim);

void dealloc(htable **h);

void add(htable *h, char *str);

void hremove(htable *h, char *str);

void clear(htable *h);

void find(htable *h, char *str, char *out_file);

int exists(htable *h, char *str);

void print_bucket(htable *h, int index, char *out_file);

void print(htable *h, char *out_file);

void resize_double(htable **h);

void resize_halve(htable **h);

#endif
