#include <stdio.h>
#define N 200


char delchar(char a[],int c)
{
    for()
}
int main()
{
    int n = 0, i;
    char ch, a[N];
    while((ch = getchar()) != '\n')
    {
        a[n] = ch;
        n++;
    }
    for(i = 0; i < n; i++)
    {
        printf("%c", delchar(a, i));
    }
    return 0;
    
}