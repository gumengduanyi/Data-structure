#include <stdio.h>



void swap(int *a, int *b);
int main(void)
{
    int a, b;
    printf("Please input the first integer:\n");
    scanf("%d", &a);
    printf("Please input the second integer:\n");
    scanf("%d", &b);

    printf("a=%d,b=%d\n", a, b);
    swap(&a, &b);
    printf("a=%d, b=%d\n", a, b);
    return 0;
}
void swap(int *p, int *m)
{
    int temp;
    temp = *p;
    *p = *m;
    *m = temp;
}