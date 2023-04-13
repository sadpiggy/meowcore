#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"
#include "buddy_system.h"
#include "slab.h"

#define PAGE_SIZE   (4 * 1024)

int main() {
    int physical_page_size = (sizeof(struct physical_page));
    int MAX_ORDER = 10;
    const size_t ALIGNED_SIZE = (1 << MAX_ORDER) * PAGE_SIZE;
    void *aligned_memory;
    int result = posix_memalign(&aligned_memory, ALIGNED_SIZE, ALIGNED_SIZE);
    struct physical_page* pages = (struct physical_page*)aligned_memory;
    struct buddy_system buddy;
    struct physical_page *page;
    struct physical_page *page2;

    // 初始化buddy分配器
    buddy_init(&buddy, pages, (1 << MAX_ORDER));

    page = list_first_entry(&buddy.free_area[1],struct physical_page,list);
    printf("firstPage==%lu\n",(uint64_t)pages);

    // struct physical_page *test_page = buddy_alloc_pages(&buddy,0);
    // printf("page==%lu\n",test_page);

    slab_init(&buddy);

    printf("endInit\n");

    // 分配和释放内存
    for (int times = 1; times <= 10; times++){
        void *ptr1 = alloc_in_slab(128);
        void *ptr2 = alloc_in_slab(256);
        void *ptr3 = alloc_in_slab(32);

        printf("Allocated memory: %p, %p, %p\n", ptr1, ptr2, ptr3);

        free_in_slab(ptr1);
        free_in_slab(ptr2);
        free_in_slab(ptr3);

        // 分配和释放内存
        void *ptr4 = alloc_in_slab(128);
        void *ptr5 = alloc_in_slab(256);
        void *ptr6 = alloc_in_slab(32);

        printf("Allocated memory: %p, %p, %p\n", ptr4, ptr5, ptr6);

        free_in_slab(ptr4);
        free_in_slab(ptr5);
        free_in_slab(ptr6);
    }


    return 0;
}
