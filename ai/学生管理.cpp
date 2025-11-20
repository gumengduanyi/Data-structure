#include <stdio.h>
#include <string.h>
#define N 50
#define M 100

void data(int x, int n, char x1[][N], char x2[][N], int c[][N]) {
    printf("输入学号，姓名，%d科成绩：\n", n - 2);
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

void sort1(int x, int n, char x1[][N], char x2[][N], int c[][N]) 
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

void sort2(int x, int n, char x1[][N], char x2[][N], int c[][N]) 
{
    int i, j, p, t;
    char m1[N];
    for (i = 0; i < x - 1; i++) 
    {
        p = i;
        for (j = i + 1; j < x; j++) {
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

void students(int x, int n, char x1[][N], char x2[][N], int c[][N]) 
{
    printf("编号      学号            姓名");
    for (int i = 1; i <= n - 2; i++) printf(" 科%d", i);
    printf(" 平均 总分\n");

    for(int i = 0; i < x; i++) 
    {
        printf("%4d %15s %15s", i + 1, x1[i], x2[i]);
        for(int j = 0; j < n; j++)
            printf(" %4d", c[i][j]);
        printf("\n");
    }
}

void find(int x, int n, char x1[][N], char x2[][N], int c[][N]) 
{
    char key[N];
    int found = 0;
    printf("请输入要查找的学号或姓名：");
    scanf("%s", key);
    for (int i = 0; i < x; i++) 
    {
        if (strcmp(x1[i], key) == 0 || strcmp(x2[i], key) == 0) 
        {
            printf("找到学生：\n");
            printf("学号：%s 姓名：%s\n", x1[i], x2[i]);
            printf("成绩：");
            for (int j = 0; j < n; j++) 
            {
                printf("%d ", c[i][j]);
            }
            printf("\n");
            found = 1;
        }
    }
    if (!found) 
    {
        printf("没有找到该学生。\n");
    }
}

int main() 
{
    int x = 0, n = 0;
    char x1[M][N], x2[M][N];
    int c[M][N];
    int entered = 0;

    while (1) {
        printf("\n==== 学生成绩管理系统 ====\n");
        printf("1. 输入学生数据\n");
        printf("2. 按总分排序\n");
        printf("3. 查找学生信息\n");
        printf("4. 打印所有学生信息\n");
        printf("0. 退出程序\n");
        printf("请输入选项：");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("请输入学生人数：");
                scanf("%d", &x);
                printf("请输入学科数：");
                scanf("%d", &n);
                n += 2; 
                data(x, n, x1, x2, c);
                entered = 1;
                break;

            case 2:
                if (!entered) 
                {
                    printf("请先输入学生数据！\n");
                    break;
                }
                sort1(x, n, x1, x2, c);
                sort2(x, n, x1, x2, c);
                printf("排序完成。\n");
                break;

            case 3:
                if (!entered) 
                {
                    printf("请先输入学生数据！\n");
                    break;
                }
                find(x, n, x1, x2, c);
                break;

            case 4:
                if (!entered) 
                {
                    printf("请先输入学生数据！\n");
                    break;
                }
                students(x, n, x1, x2, c);
                break;

            case 0:
                printf("退出程序。\n");
                return 0;

            default:
                printf("无效选项，请重新输入。\n");
        }
    }
    return 0;
}
