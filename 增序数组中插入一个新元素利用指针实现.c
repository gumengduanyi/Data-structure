#include <stdio.h>
#define N 6




void add_a(int *a, int n)
{
    int i, j, t;
    for(i = 0; i < N; i++)
    {
        if(n < *(a + i))
            break;
        
    }
    for(j = N -1; j > i; j--)
    {
        *(a + j) = *(a + j - 1);
    }
    *(a + i) = n;
    
}
int main(void)
{
    int a[N], n, i;
    printf("input array data:");
    for(i = 0; i < 5; i++)
    scanf("%d", &a[i]);
    printf("input insert data:");
    scanf("%d", &n);
    add_a(a, n);
     for(i = 0; i < N; i++)
        printf("%d ", a[i]);
    return 0;
}