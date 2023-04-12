#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"
#include "buddy_system.h"


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
    // printf("firstPage==%lu\n",page);

    // 分配2个物理页
    page = buddy_alloc_pages(&buddy, 9);
    if (page != NULL) {
        printf("Allocated 1<<9 pages==%lu\n",(uint64_t)page);

    }

    // 分配8个物理页
    page2 = buddy_alloc_pages(&buddy, 9);
    if (page2 != NULL) {
        printf("Allocated 1<<9 pages==%lu\n",(uint64_t)page2);
       
    }
    buddy_free_pages(&buddy, page);
    buddy_free_pages(&buddy, page2);
    page = buddy_alloc_pages(&buddy, 10);
    if (page != NULL) {
        printf("Allocated 1<<10 pages%lu\n",(uint64_t)page);
        
    }

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "buddy.h"

// #define PAGE_SIZE       4096
// #define MEMPOOL_SIZE    (16 * 1024 * 1024)   // 内存池大小，16MB
// #define BUDDY_MAX_ORDER 10                   // 最大支持的分配单元大小，1<<10=1024个页

// struct memory_pool {
//     void *start;
//     long size;
// };

// struct memory_pool create_memory_pool(long size) {
//     int MAX_ORDER = 10;
//     const size_t ALIGNED_SIZE = (1 << MAX_ORDER) * PAGE_SIZE;
//     void *start;
//     int result = posix_memalign(&start, ALIGNED_SIZE, size);
//     // void *start = malloc(size);
//     struct memory_pool pool = {start, size};
//     return pool;
// }

// void destroy_memory_pool(struct memory_pool *pool) {
//     free(pool->start);
//     memset(pool, 0, sizeof(*pool));
// }

// int main() {
//     // 创建内存池
//     struct memory_pool pool = create_memory_pool(MEMPOOL_SIZE);

//     // 初始化buddy分配器
//     struct buddy_system buddy;
//     struct physical_page *pages = (struct physical_page *)pool.start;
//     buddy_init(&buddy, pages, MEMPOOL_SIZE / PAGE_SIZE);

//     // 分配和释放物理页，验证buddy分配器的正确性
//     struct physical_page *page;
//     for (int i = 0; i < 1000; i++) {
//         // 随机分配1-1024个页
//         int order = rand() % BUDDY_MAX_ORDER + 1;
//         page = buddy_alloc_pages(&buddy, order);
//         if (page != NULL) {
//             printf("Allocated %d pages\n", 1 << order);
//             // memset(page, 0, ( 1 << order) * PAGE_SIZE);
//             buddy_free_pages(&buddy, page);
//         }
// }

// // 销毁内存池
// destroy_memory_pool(&pool);

// return 0;
// }