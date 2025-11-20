#include <stdio.h>
#include <string.h>

void delchar(char *s, char c) 
{
    int i, j = 0;
    for (i = 0; s[i] != '\0'; i++) 
    {
        if (s[i] != c) 
        {
            s[j++] = s[i];
        }
    }
    s[j] = '\0';
}

int main() 
{
    char s[1000];
    char c;

    printf("Input a string:");
    fgets(s, sizeof(s), stdin);
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') 
    {
        s[len - 1] = '\0';
    }
    printf("Input a char:");
    c = getchar();
    delchar(s, c);
    printf("After deleted,the string is:%s", s);

    return 0;
}
