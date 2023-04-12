#ifndef LIST_h
#define LIST_h

struct list_head {
    struct list_head *next, *prev;
};

#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)


// 将新元素添加到链表的尾部
static inline void list_add_tail(struct list_head *new_node, struct list_head *head) {
    struct list_head *prev = head->prev;
    
    new_node->next = head;
    new_node->prev = prev;
    prev->next = new_node;
    head->prev = new_node;
}

// 从链表中删除元素
static inline void list_del(struct list_head *entry) {
    struct list_head *prev = entry->prev;
    struct list_head *next = entry->next;
    next->prev = prev;
    prev->next = next;
}

// 检查链表是否为空
static inline int list_empty(const struct list_head *head) {
    return head->next == head;
}

// 获取链表的第一个元素
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

// 获取指定类型的结构体的指针
// type为返回的type，ptr为绑定的list指针，member为list的名字
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define list_for_each_entry(pos, head, member)                              \
    for (pos = list_entry((head)->next, typeof(*pos), member);              \
        &pos->member != (head);                                            \
        pos = list_entry(pos->member.next, typeof(*pos), member))


#endif