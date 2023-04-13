#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "PT.h"
#include "buddy_system.h"
#include "slab.h"


slab_head *slabs[SLAB_MAX_ORDER - SLAB_MIN_ORDER + 1];


struct buddy_system* buddy_for_slab = NULL;

void slab_init(struct buddy_system* buddy) {
    buddy_for_slab = buddy;
    // printf("before init\n");
    for (int order = SLAB_MIN_ORDER; order <= SLAB_MAX_ORDER; order++) {
        // printf("order==%d\n",order);
        init_slab_cache(order, SLAB_INIT_SIZE);
    }
    // printf("end init\n");
}

void init_slab_cache(int order, int size) {
    // printf("size==%d\n",get_order(size));
    // printf("beforeAlloc\n");
    struct physical_page *pages = buddy_alloc_pages(buddy_for_slab,get_order(size));
    // if(pages==NULL)
    //     printf("isNull\n");
    // else
    //     printf("notNull\n");
    assert(pages != NULL);
    // printf("page==%lu,order==%d\n",pages,pages->order);

    slab_head *new_slab = (slab_head *)pages;
    new_slab->order = order;
    new_slab->next_slab = NULL;

    int num_slots = (PAGE_SIZE * size) / (1 << order) ;
    slot *prev_slot = NULL;
    slot *current_slot = (slot *)((uintptr_t)new_slab + (1 << order));

    for (int i = 1; i < num_slots; i++) {
        if (prev_slot) {
            prev_slot->next_free = current_slot;
        }
        prev_slot = current_slot;
        current_slot->next_free = NULL;
        current_slot = (slot *)((uintptr_t)current_slot + (1 << order));
        //如果currenr_slot位于最后一个slot，则会导致下一个页的值被修改；但是如果不这么写，则会导致需要NULL的地方没有NULL
    }
    // printf("diff==%lu\n",(uint64_t)current_slot - (uint64_t)pages);

    new_slab->free_list = (void *)((uintptr_t)new_slab + (1 << order));

    slab_head *existing_slab = slabs[order - SLAB_MIN_ORDER];
    if (existing_slab) {
        while (existing_slab->next_slab) {
            existing_slab = existing_slab->next_slab;
        }
        existing_slab->next_slab = new_slab;
    } else {
        slabs[order - SLAB_MIN_ORDER] = new_slab;
    }
}

void* alloc_in_slab(int size) {
    int order = SLAB_MIN_ORDER;
    while (order <= SLAB_MAX_ORDER && (1 << order) < size) {
        order++;
    }

    slab_head *current_slab = slabs[order - SLAB_MIN_ORDER];
    while (current_slab && !current_slab->free_list) {
        current_slab = current_slab->next_slab;
    }

    if (!current_slab) {
        init_slab_cache(order, SLAB_INIT_SIZE);
        current_slab = slabs[order - SLAB_MIN_ORDER];
    }

    slot *allocated_slot = (slot *)current_slab->free_list;
    current_slab->free_list = allocated_slot->next_free;
    return (void *)allocated_slot;
}

void free_in_slab(void *addr) {
    uintptr_t aligned_addr = (uintptr_t)addr & (~(uintptr_t)(PAGE_SIZE - 1));
    slab_head *current_slab = (slab_head *)aligned_addr;
    int order = current_slab->order;

    slot *freed_slot = (slot *)addr;
    freed_slot->next_free = current_slab->free_list;
    current_slab->free_list = (void *)freed_slot;
}