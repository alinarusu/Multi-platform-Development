#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"
#include "utils.h"

#define MAX 20001

void apply(char *cmd[3], htable **h)
{
    if (!strcmp(cmd[0], "add"))
        add(*h, cmd[1]);
    else if (!strcmp(cmd[0], "remove"))
        hremove(*h, cmd[1]);
    else if (!strcmp(cmd[0], "clear"))
        clear(*h);
    else if (!strcmp(cmd[0], "find"))
        find(*h, cmd[1], cmd[2]);
    else if (!strcmp(cmd[0], "print_bucket"))
        print_bucket(*h, atoi(cmd[1]), cmd[2]);
    else if (!strcmp(cmd[0], "print"))
        print(*h, cmd[1]);
    else if (!strcmp(cmd[0], "resize") && !strcmp(cmd[1], "double"))
        resize_double(h);
    else if (!strcmp(cmd[0], "resize") && !strcmp(cmd[1], "halve"))
        resize_halve(h);
}

void parse_file(htable **h, char *in_file)
{
    char buf[MAX], *cmd[3], seps[] = " \r\n\t";
    FILE *f = NULL;

    if (in_file) {
        f = fopen(in_file, "rt");
        DIE(f == NULL, "parse_file error");
    } else {
        f = stdin;
    }

    while (fgets(buf, MAX, f) != NULL) {
        cmd[0] = NULL;
        cmd[1] = NULL;
        cmd[2] = NULL;

        cmd[0] = strtok(buf, seps);
        if (!cmd[0])
            continue;

        cmd[1] = strtok(NULL, seps);
        if (cmd[1])
            cmd[2] = strtok(NULL, seps);

        apply(cmd, h);
    }

    if (in_file)
        fclose(f);
}

int main(int argc, char **argv)
{
    int i;
    htable *h = NULL;
    alloc(&h, atoi(argv[1]));

    if (argc == 2) {
        parse_file(&h, NULL);
    } else {
        for (i = 2; i < argc; i++)
            parse_file(&h, argv[i]);
    }

    dealloc(&h);
    return 0;
}
