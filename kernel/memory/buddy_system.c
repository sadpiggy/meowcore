#include "buddy_system.h"
#include <math.h>
// #define physical_page_size (sizeof(struct physical_page))
const uint64_t physical_page_size = PAGE_SIZE;
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
            start_page = (struct physical_page *)((uint64_t)start_page +   (1 << order)*physical_page_size);
            remaining_pages -= (1 << order);
        } else {
            order--;
        }
    }
}

struct physical_page *buddy_alloc_pages(struct buddy_system *buddy, int order) {
    for (int i = order; i < NR_QUANTUM; ++i) {
        if (!list_empty(&buddy->free_area[i])) {
            // printf("order==%lu\n",order);
            struct physical_page *page = list_first_entry(&buddy->free_area[i], struct physical_page, list);
            // printf("page==%lu order==%d\n",page,page->order);
            list_del(&page->list);
            page->order = order;
            int remaining_order = order;
            struct physical_page *remaining_page = (struct physical_page *)((uintptr_t)page + (physical_page_size << order));
            while (remaining_order < i) {
                // printf("remain_page==%lu,order==%d\n",remaining_page,remaining_order);
                remaining_page->order = remaining_order;
                list_add_tail(&remaining_page->list, &buddy->free_area[remaining_order]);
                remaining_page = (struct physical_page *)((uint64_t)remaining_page +  (1 << (remaining_order))*physical_page_size);
                remaining_order++;
            }
            // printf("page==%lu returnpage\n",page);

            return page;
        }
    }

    return NULL;
}

int calculate_order(struct buddy_system *buddy, struct physical_page *page) {
    uintptr_t base_addr = (uintptr_t)page;
    uintptr_t buddy_start_addr = (uintptr_t)buddy->start_page;

    for (int order = 0; order <= NR_QUANTUM-1; ++order) {
        uintptr_t mask = (1 << (order + 12)) - 1;

        if ((base_addr ^ buddy_start_addr) & mask) {
            return order - 1;
        }
    }

    return NR_QUANTUM-1;
}

void buddy_free_pages(struct buddy_system *buddy, struct physical_page *page,int page_order) {
    // order不能等于page->order
    // printf("calorder==%d\n",calculate_order(buddy,page));
    int order = page->order;
    order = page_order;

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
