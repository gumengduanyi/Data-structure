#include <stdio.h>
#include <string.h>




void delete_substring(char *str, char *sub) 
{
    int i, j, len = strlen(sub);
    for(i = 0; str[i] != '\0'; i++) 
    {
        if(strncmp(&str[i], sub, len) == 0) 
        {
            for(j = i; str[j + len] != '\0'; j++) 
            {
                str[j] = str[j + len];
            }
            str[j] = '\0';
            i--;
        }
    }
}

void run() 
{
    char str[100], sub[10];

    printf("Input a string:");
    fgets(str, sizeof(str), stdin);

    printf("Input a char:");
    fgets(sub, sizeof(sub), stdin);
    sub[strcspn(sub, "\n")] = '\0';

    delete_substring(str, sub);

    printf("After deleted,the string is:%s", str);
}

int main() 
{
    run();
    return 0;
}
