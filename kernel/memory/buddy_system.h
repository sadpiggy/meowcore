#ifndef BUDDY_SYSTEM_H
#define BUDDY_SYSTEM_H
#include "list.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "PT.h"
#define NR_QUANTUM 11
struct physical_page {
    struct list_head list;
    int order;
};


struct buddy_system {
    struct physical_page *start_page;
    long page_num;
    struct list_head free_area[11];
};


void buddy_init(struct buddy_system *buddy, struct physical_page *start_page, long page_num);
//得到的是pages的基地址
struct physical_page* buddy_alloc_pages(struct buddy_system *buddy, int order);
void buddy_free_pages(struct buddy_system *buddy, struct physical_page *page,int page_order);
int get_order(int size);
#endif