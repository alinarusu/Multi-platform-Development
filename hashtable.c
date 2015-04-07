#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"
#include "utils.h"
#include "hash.h"

// clear a specific bucket. elem is head of the list

void free_elem(node *elem)
{
    free(elem->str);
    free(elem);
}

void clear_bucket(node *elem)
{
    if (elem == NULL)
        return;

    clear_bucket(elem->succ);
    free_elem(elem);
}

// look for an element in the hash table. return 1 if it exists or 0 if not
int exists(htable *h, char *str)
{
    int index = hash(str, h->dim);
    node *elem = NULL;

    for (elem = h->v[index]; elem; elem = elem->succ)
        if (!strcmp(elem->str, str))
            return 1;
    return 0;
}

// allocate memory for a new hash table of size <dim> and assign it to h.
void alloc(htable **h, int dim)
{
    int i;
    *h = malloc(sizeof(htable));
    DIE(*h == NULL, "alloc error");
    (*h)->v = malloc(dim * sizeof(node*));

    for (i = 0; i < dim; i++) {
        (*h)->v[i] = NULL;
    }
    (*h)->dim = dim;
}

// free all memory used up by the hash table
void dealloc(htable **h)
{
    clear(*h);
    free((*h)->v);
    free(*h);
    *h = NULL;
}

// add an element to the hash table if it wasn't there already
void add(htable *h, char *str)
{
    int index = 0;
    node *elem = NULL;

    // test if it already exists
    if (exists(h, str))
        return;

    index = hash(str, h->dim);
    elem = h->v[index];

    // if it's the first element in this bucket
    if (elem == NULL) {
        elem = malloc(sizeof(node));
        elem->str = malloc(strlen(str) + 1);
        strncpy(elem->str, str, strlen(str) + 1);
        elem->succ = NULL;
        h->v[index] = elem;
        return;
    }

    // get to the last element
    while (elem->succ)
        elem = elem->succ;

    // insert at the end
    elem->succ = malloc(sizeof(node));
    elem->succ->str = malloc(strlen(str) + 1);
    strncpy(elem->succ->str, str, strlen(str) + 1);
    elem->succ->succ = NULL;
}

// remove an element from a hash table. If it doesn't exist the function
// just exits normally.
void hremove(htable *h, char *str)
{
    int index = hash(str, h->dim);
    node *elem = NULL, *tmp = NULL;

    if (h->v[index] == NULL)
        return;

    // test first element
    if (!strcmp(h->v[index]->str, str)) {
        tmp = h->v[index];
        h->v[index] = h->v[index]->succ;

        // free mem
        free_elem(tmp);
        return;
    }

    // interate through the list
    for (elem = h->v[index]; elem->succ; elem = elem->succ) {
        if (strcmp(elem->succ->str, str) == 0) {
            tmp = elem->succ;
            elem->succ = elem->succ->succ;

            // free mem
            free_elem(tmp);
            return;
        }
    }
}

// clear all buckets
void clear(htable *h)
{
    int i;

    for (i = 0; i < h->dim; i++) {
        clear_bucket(h->v[i]);
        h->v[i] = NULL;
    }
}

// Look for an element in the hash table and print True/False in a file whose
// name is given as parameter. If out_file is NULL the result is printed to
// stdout.
void find(htable *h, char *str, char *out_file)
{
    FILE *f = NULL;
    char result[] = "False\n";

    if (out_file) {
        f = fopen(out_file, "a+");
        DIE(f == NULL, "find error");
    }

    if (exists(h, str))
        strcpy(result, "True\n");

    if (out_file) {
            fprintf(f, "%s", result);
            fclose(f);
    } else {
            printf("%s", result);
    }
}

// Print a specific bucket to a given file or to stdout, if out_file is NULL.
// The bucket to be printed is h->v[index]
void print_bucket(htable *h, int index, char *out_file)
{
    FILE *f = NULL;
    node *elem = NULL;

    if (out_file != NULL) {
        f = fopen(out_file, "a+");
        DIE(f == NULL, "print bucket error");
    }

    for (elem = h->v[index]; elem; elem = elem->succ) {
        if (out_file)
            fprintf(f, "%s ", elem->str);
        else
            printf("%s ", elem->str);
    }

    if (out_file) {
        fprintf(f, "\n");
        fclose(f);
    } else {
        printf("\n");
    }
}

// print whole hash table to a file or to stdout (if out_file == NULL)
void print(htable *h, char *out_file)
{
    int i;
    for (i = 0; i < h->dim; i++) {
        if (h->v[i])
            print_bucket(h, i, out_file);
    }
}

// double the size  of a hash table
void resize_double(htable **h)
{
    htable *h_new = NULL;
    int i;
    node *elem;

    alloc(&h_new, (*h)->dim * 2);

    for (i = 0; i < (*h)->dim; i++) {
        for (elem = (*h)->v[i]; elem; elem = elem->succ) {
            add(h_new, elem->str);
        }
    }

    dealloc(h);
    *h = h_new;
}

// halve the size of a hash table
void resize_halve(htable **h)
{
    htable *h_new = NULL;
    int i;
    node *elem;

    alloc(&h_new, (*h)->dim / 2);

    for (i = 0; i < (*h)->dim; i++) {
        for (elem = (*h)->v[i]; elem; elem = elem->succ) {
            add(h_new, elem->str);
        }
    }

    dealloc(h);
    *h = h_new;
}
