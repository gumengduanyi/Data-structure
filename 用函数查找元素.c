#include <stdio.h>




int find(int arr[], int n, int target) 
{
    int i;
    for(i = 0; i < n; i++) 
    {
        if(arr[i] == target) 
        {
            return i;
        }
    }
    return -1;
}

void run()
{
    int n, x;
    printf("Input n: ");
    scanf("%d", &n);

    int a[n];
    printf("Input %d integers: ", n);
    for(int i = 0; i < n; i++) 
    {
        scanf("%d", &a[i]);
    }

    printf("Input x: ");
    scanf("%d", &x);

    int index = find(a, n, x);
    if(index != -1)
        printf("index = %d\n", index);
    else
        printf("Not found\n");
}

int main() 
{
    run();
    return 0;
}
