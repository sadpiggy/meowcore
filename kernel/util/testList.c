#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct person {
    int id;
    char name[20];
    struct list_head list;
};

int main() {
    struct person p1 = {1, "Alice", {NULL, NULL}};
    struct person p2 = {2, "Bob", {NULL, NULL}};
    struct person p3 = {3, "Charlie", {NULL, NULL}};

    struct list_head *person_list = (struct list_head *) malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(person_list);

    list_add_tail(&p1.list, person_list);
    list_add_tail(&p2.list, person_list);
    list_add_tail(&p3.list, person_list);


    struct person *entry;

    entry = list_first_entry(person_list,struct person, list);
    printf("-------Person id = %d, name = %s\n", entry->id, entry->name);

    list_for_each_entry(entry, person_list, list) {
        printf("Person id = %d, name = %s\n", entry->id, entry->name);
    }

    struct person *to_be_deleted = &p2;
    list_del(&to_be_deleted->list);
    // free(to_be_deleted);

    printf("After deleting person \"Bob\":\n");
    list_for_each_entry(entry, person_list, list) {
        printf("Person id = %d, name = %s\n", entry->id, entry->name);
    }
    return 0;
}