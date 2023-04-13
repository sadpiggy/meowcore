#ifndef SLAB_H
#define SLAB_H
#define SLAB_MIN_ORDER 4
#define SLAB_MAX_ORDER 8
#define SLAB_INIT_SIZE 1

typedef struct slot {
    struct slot *next_free;
} slot;
//uddy_system分配给它的页面被划分为很多slot，slab位于第一个slot上
typedef struct slab {
    void *free_list;
    struct slab *next_slab;
    int order;
} slab_head;

void slab_init(struct buddy_system* buddy);
//分配总共大小为((size)*page_size)的内存给slab，slab将其分为单位为(1<<order)块。其中，order需要为2幂次
void init_slab_cache(int order, int size);
void* alloc_in_slab(int size);
void free_in_slab(void *addr);


#endif