#include "buddy_system.h"
#include <math.h>
#define physical_page_size (sizeof(struct physical_page))
// 返回order,1*(2^n) + 
int get_order(int size) {
    int order = 0;
    while ((size = size >> 1)) {
        order++;
    }
    return order;
}


void buddy_init(struct buddy_system *buddy, struct physical_page *start_page, long page_num) {
    buddy->start_page = start_page;
    buddy->page_num = page_num;
    for (int i = 0; i < NR_QUANTUM; ++i) {
        INIT_LIST_HEAD(&buddy->free_area[i]);
    }

    int order = NR_QUANTUM - 1;
    long remaining_pages = page_num;
    while (remaining_pages > 0) {
        if (remaining_pages >= (1 << order)) {
            start_page->order = order;
            list_add_tail(&start_page->list, &buddy->free_area[order]);
            start_page += (1 << order)*physical_page_size;
            remaining_pages -= (1 << order);
        } else {
            order--;
        }
    }
}

struct physical_page *buddy_alloc_pages(struct buddy_system *buddy, int order) {
    for (int i = order; i < NR_QUANTUM; ++i) {
        if (!list_empty(&buddy->free_area[i])) {
            
            struct physical_page *page = list_first_entry(&buddy->free_area[i], struct physical_page, list);
            list_del(&page->list);
            page->order = order;
            int remaining_order = i - 1;
            struct physical_page *remaining_page = (struct physical_page *)((uintptr_t)page + (physical_page_size << order));
            while (remaining_order >= order) {
                remaining_page->order = remaining_order;
                list_add_tail(&remaining_page->list, &buddy->free_area[remaining_order]);
                remaining_page += (1 << (remaining_order))*physical_page_size;
                remaining_order--;
            }

            return page;
        }
    }

    return NULL;
}


void buddy_free_pages(struct buddy_system *buddy, struct physical_page *page) {
    int order = page->order;

    while (order < 10) {
        struct physical_page *buddy_page = (struct physical_page *)((uintptr_t)page ^ (physical_page_size << order));
        int found = 0;
        struct physical_page *pos;
        list_for_each_entry(pos, &buddy->free_area[order], list) {
            if (pos == buddy_page) {
                found = 1;
                break;
            }
        }

        if (found) {
            
            list_del(&buddy_page->list);
            page = (struct physical_page *)((uintptr_t)page & ~((uintptr_t)(physical_page_size << order)));
            order++;
        } else {
            page->order = order;
            list_add_tail(&page->list, &buddy->free_area[order]);
            break;
        }
    }

    if (order == 10) {
        page->order = order;
        list_add_tail(&page->list, &buddy->free_area[order]);
    }
}
