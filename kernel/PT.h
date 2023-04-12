#ifndef PT_H
#define PT_H
#include <stdint.h>
// pte: [9:0]标志位；[53:10]ppn
// vpn:9,9,9,9
// ppn:17,9,9,9
#define RISCV_PG_LEVELS 4
#define RISCV_PG_SHIFT 12 
#define RISCV_PG_SIZE (1UL << RISCV_PG_SHIFT) 

typedef struct sv48_pte {
    uint64_t val;
} SV48_PTE;

#define SV48_PTE_V     (1UL << 0)  // 有效（Valid）
#define SV48_PTE_R     (1UL << 1)  // 可读（Read）
#define SV48_PTE_W     (1UL << 2)  // 可写（Write）
#define SV48_PTE_X     (1UL << 3)  // 可执行（Execute）
#define SV48_PTE_U     (1UL << 4)  // 用户模式（User）
#define SV48_PTE_G     (1UL << 5)  // 全局（Global）
#define SV48_PTE_A     (1UL << 6)  // 访问（Accessed）
#define SV48_PTE_D     (1UL << 7)  // 脏（Dirty）
#define SV48_PTE_SOFT1 (1UL << 8)  // 软件可用位1（Software use 1）
#define SV48_PTE_PPN_SHIFT 10      // 物理页号（Physical Page Number）偏移
#define SV48_PTE_PPN_MASK  ((1UL << 44) - 1)

// PGD,PUD,PMD,PTE
#define SV48_VA_PGD_INDEX(va) (((va) >> 39) & 0x1FF)
#define SV48_VA_PUD_INDEX(va) (((va) >> 30) & 0x1FF)
#define SV48_VA_PMD_INDEX(va) (((va) >> 21) & 0x1FF)
#define SV48_VA_PTE_INDEX(va) (((va) >> 12) & 0x1FF)
#define SV48_VA_OFFSET(va) ((va) & 0xFFF)//得到page_offset

#define SV48_PA(pte) ((((pte) >> 10) & SV48_PTE_PPN_MASK))//<<page_shift，得到物理地址；<<pte_shift，得到下一级页表地址

#define PTE_SIZE (8UL)
#define PTE_SHIFT 3
#define PAGE_TABLE_PTE_NUM (1UL << 9)
#define PAGE_TABLE_SIZE (PAGE_TABLE_PTE_NUM * PTE_SIZE)

#endif