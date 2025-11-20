#include <stdio.h>
#define N 20


int len(char c[], int b)
{
    int i, sum = 0;
    for(i = 0; i < b; i++)
    {
        if(c[i] != '\0')
            sum++;
        else 
            break;
    }
    return(sum);
}

int main()
{
    int a = 0, l;
    char c[N], ch;
    while((ch = getchar()) != '\n') 
    {
        c[a] = ch;
        a++;
    }
    l = len(c, a);
    printf("%d", l);
    return 0;
}