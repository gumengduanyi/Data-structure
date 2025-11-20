#include <stdio.h>
#define N 10




int find(int *a, int x) 
{
    int i;
    for(i = 0; i < N; i++) 
    {
        if(x == *(a + i)) 
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    int x, a[N];
    printf("input array data:\n");
    for(int i = 0; i < N; i++) 
    {
        scanf("%d", &a[i]);
    }
    printf("input search data:\n");
    scanf("%d", &x);
    int index = find(a, x);
    if(index != -1)
        printf("%d\n", index);
    else
        printf("No!\n");
}
