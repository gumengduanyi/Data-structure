#include <stdio.h>
#include <string.h>

#define N 50  
#define M 3   

void input_student_data(int x, int n, char x1[][N], char x2[][N], int c[][N]) 
{
    printf("输入学号，姓名，%d科成绩\n", n - 2);
    for(int i = 0; i < x; i++) 
    {
        scanf("%s", x1[i]);
        scanf("%s", x2[i]);
        for(int j = 0; j < n - 2; j++) 
        {
            scanf("%d", &c[i][j]);
        }
        c[i][n - 1] = 0;
        for(int j = 0; j < n - 2; j++) 
        {
            c[i][n - 1] += c[i][j];
        }
        c[i][n - 2] = c[i][n - 1] / (n - 2);
    }
}

void sort_by_total_score(int x, int n, char x1[][N], char x2[][N], int c[][N]) 
{
    int i, j, p, t;
    char m1[N];
    for (i = 0; i < x - 1; i++) 
    {
        p = i;
        for (j = i + 1; j < x; j++) 
        {
            if (c[p][n - 1] < c[j][n - 1]) 
            {
                p = j;
            }
        }
        if (p != i) 
        {
            for(int k = 0; k < n; k++) 
            {
                t = c[p][k];
                c[p][k] = c[i][k];
                c[i][k] = t;
            }
            strcpy(m1, x1[p]); strcpy(x1[p], x1[i]); strcpy(x1[i], m1);
            strcpy(m1, x2[p]); strcpy(x2[p], x2[i]); strcpy(x2[i], m1);
        }
    }
}

void sort_by_id_if_same_score(int x, int n, char x1[][N], char x2[][N], int c[][N]) 
{
    int i, j, p, t;
    char m1[N];
    for (i = 0; i < x - 1; i++) 
    {
        p = i;
        for (j = i + 1; j < x; j++) 
        {
            if (c[i][n - 1] == c[j][n - 1] && strcmp(x1[p], x1[j]) > 0) 
            {
                p = j;
            }
        }
        if (p != i) 
        {
            for(int k = 0; k < n; k++) 
            {
                t = c[i][k];
                c[i][k] = c[p][k];
                c[p][k] = t;
            }
            strcpy(m1, x1[i]); strcpy(x1[i], x1[p]); strcpy(x1[p], m1);
            strcpy(m1, x2[i]); strcpy(x2[i], x2[p]); strcpy(x2[p], m1);
        }
    }
}

void print_students(int x, int n, char x1[][N], char x2[][N], int c[][N]) 
{
    for(int i = 0; i < x; i++) 
    {
        printf("%4d", i + 1);
        printf("%15s%15s", x1[i], x2[i]);
        for(int j = 0; j < n; j++)
            printf("%5d", c[i][j]);
        printf("\n");
    }
}

int main() 
{
    int x, n;
    printf("请输入学生人数：");
    scanf("%d", &x);
    printf("请输入学科数：");
    scanf("%d", &n);
    n += 2;

    char x1[x][N], x2[x][N];
    int c[x][n];

    input_student_data(x, n, x1, x2, c);
    sort_by_total_score(x, n, x1, x2, c);
    sort_by_id_if_same_score(x, n, x1, x2, c);
    print_students(x, n, x1, x2, c);

    return 0;
}
