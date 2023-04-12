#include <stdio.h>
#include <stdint.h>
struct pig {
    int a[10];
};
int main(){
    struct pig* ptr = 1200;
    printf("ptr==%lu",ptr);
    printf("ptr==%lu",ptr+10);
    printf("ptr==%lu",(uint64_t)ptr + 10);
}