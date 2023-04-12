#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include "PT.h"
// 1UL<<9
typedef struct sv48_address_space {
    SV48_PTE *pgd;  // 顶层页表（Page Global Directory）指针
} SV48AddressSpace;


#endif