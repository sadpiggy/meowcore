#include <stdio.h>
#include <stdlib.h>
#include "buddy_system.h"

#define PAGE_SIZE   (4 * 1024)

int main() {
    int MAX_ORDER = 10;
    const size_t ALIGNED_SIZE = (1 << MAX_ORDER) * PAGE_SIZE;

    void *aligned_memory;
    int result = posix_memalign(&aligned_memory, ALIGNED_SIZE, ALIGNED_SIZE);
    struct physical_page* pages = (struct physical_page*)aligned_memory;
    struct buddy_system buddy;
    struct physical_page *page;
    // printf("getorder%d %d\n",get_order(2 * PAGE_SIZE),get_order(2));

    // 初始化buddy分配器
    printf("physical_page size == %lu %lu\n",sizeof(struct physical_page),sizeof(pages));
    printf("before init\n");
    buddy_init(&buddy, pages, 128);
    printf("after init\n");

    // 分配2个物理页1*(2^13)
    page = buddy_alloc_pages(&buddy, 1);
    printf("after alloc\n");
    if (page != NULL) {
        void *page_addr = (void *)((unsigned long)pages + ((unsigned long)page - (unsigned long)pages) * PAGE_SIZE);
        printf("Allocated 2 pages starting at address %p\n", page_addr);
        // buddy_free_pages(&buddy, page);
        printf("pageOrder==%d\n",page->order);
    }

    // 分配8个物理页
    page = buddy_alloc_pages(&buddy, 3);
    if (page != NULL) {
        void *page_addr = (void *)((unsigned long)pages + ((unsigned long)page - (unsigned long)pages) * PAGE_SIZE);
        printf("Allocated 8 pages starting at address %p\n", page_addr);
        // buddy_free_pages(&buddy, page);
        printf("pageOrder==%d\n",page->order);
    }

    printf("end test\n");

    return 0;
}