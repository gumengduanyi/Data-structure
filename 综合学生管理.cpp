#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct student
{
    long studentID;       
    char name[20];       
    int score[3];         
    float average;        
    int sum;              
    struct student *next;
};

void menu()
{
    printf("\t 综合学生管理系统\n");
    printf("\t     1.学生信息录入\n");
    printf("\t     2.学生成绩输出\n");
    printf("\t     3.学生信息查询\n");
    printf("\t     4.修改学生信息\n");
    printf("\t     5.删除学生信息\n");
    printf("\t     6.学生信息排序\n");
    printf("\t     7.统计学生信息\n");
    printf("\t     8.保存学生信息到文件\n");
    printf("\t     9.从文件读取学生信息\n");
    printf("\t     10.退出系统\n");
}

struct student *add_students(struct student *first)
{
    int n;
    printf("请输入要录入的学生数量: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        struct student *new_student = (struct student *)malloc(sizeof(struct student));
        if (!new_student)
        {
            printf("内存分配失败！\n");
            return first;
        }

        printf("请输入学生 %d 的学号: ", i + 1);
        scanf("%ld", &new_student->studentID);
        printf("请输入学生 %d 的姓名: ", i + 1);
        scanf("%s", new_student->name);
        printf("请输入学生 %d 的三门课程成绩: ", i + 1);
        new_student->sum = 0;
        for (int j = 0; j < 3; j++)
        {
            scanf("%d", &new_student->score[j]);
            new_student->sum += new_student->score[j];
        }
        new_student->average = new_student->sum / 3.0;
        new_student->next = NULL;

        if (first == NULL)
        {
            first = new_student; 
        }
        else
        {
            struct student *last = first;
            while (last->next != NULL)
            {
                last = last->next; 
            }
            last->next = new_student; 
        }
    }

    printf("已成功录入 %d 名学生的信息！\n", n);
    return first;
}

void print_students(struct student *first)
{
    if (first == NULL)
    {
        printf("当前学生管理系统里没有学生信息可以输出！\n");
        return;
    }

    struct student *print = first;
    printf("学生信息如下:\n");
    while (print != NULL)
    {
        printf("学号: %-9ld, 姓名: %-8s, 总分: %-5d, 平均分: %-5.2f, 成绩: ", 
               print->studentID, print->name, print->sum, print->average);
        for (int i = 0; i < 3; i++)
        {
            printf("%-5d", print->score[i]);
        }
        printf("\n");
        print = print->next; 
    }
}

void query_student(struct student *first)
{
    if (first == NULL)
    {
        printf("当前学生管理系统里没有学生信息可以查询！\n");
        return;
    }

    long ID;
    printf("请输入要查询的学生学号: ");
    scanf("%ld", &ID);

    struct student *query = first;
    while (query != NULL)
    {
        if (query->studentID == ID)
        {
            printf("学号: %-9ld, 姓名: %-8s, 总分: %-5d, 平均分: %-5.2f, 成绩: ", 
                   query->studentID, query->name, query->sum, query->average);
            for (int i = 0; i < 3; i++)
            {
                printf("%-5d", query->score[i]);
            }
            printf("\n");
            return;
        }
        query = query->next;
    }
    printf("未找到学号为 %ld 的学生信息！\n", ID);
}

void modify_student(struct student *first)
{
    if (first == NULL)
    {
        printf("当前学生管理系统里没有学生信息可以修改！\n");
        return;
    }

    long ID;
    printf("请输入要修改的学生学号: ");
    scanf("%ld", &ID);

    struct student *modify = first;
    while (modify != NULL)
    {
        if (modify->studentID == ID)
        {
            printf("请输入新的姓名: ");
            scanf("%s", modify->name);
            printf("请输入新的三门课程成绩: ");
            modify->sum = 0;
            for (int i = 0; i < 3; i++)
            {
                scanf("%d", &modify->score[i]);
                modify->sum += modify->score[i];
            }
            modify->average = modify->sum / 3.0;
            printf("学生信息已修改！\n");
            return;
        }
        modify = modify->next;
    }
    printf("未找到学号为 %ld 的学生信息！\n", ID);
}

struct student *delete_student(struct student *first)
{
    if (first == NULL)
    {
        printf("当前学生管理系统里没有学生信息可以删除！\n");
        return first;
    }

    long ID;
    printf("请输入要删除的学生学号: ");
    scanf("%ld", &ID);

    struct student *cur = first, *prev = NULL;
    while (cur != NULL)
    {
        if (cur->studentID == ID)
        {
            if (prev == NULL)
            {
                first = cur->next; 
            }
            else
            {
                prev->next = cur->next; 
            }
            free(cur);
            printf("学号为 %ld 的学生信息已删除！\n", ID);
            return first;
        }
        prev = cur;
        cur = cur->next;
    }
    printf("未找到学号为 %ld 的学生信息！\n", ID);
    return first;
}

void save_to_file(struct student *first)
{
    if (first == NULL)
    {
        printf("学生管理系统暂无学生信息，无法保存到文件！\n");
        return;
    }

    FILE *fp = fopen("students.txt", "w");
    if (!fp)
    {
        printf("文件打开失败！\n");
        return;
    }

    struct student *temp = first;
    while (temp != NULL)
    {
        fprintf(fp, "%ld %s %d %d %d %d %.2f\n", temp->studentID, temp->name, 
                temp->score[0], temp->score[1], temp->score[2], temp->sum, temp->average);
        temp = temp->next;
    }

    fclose(fp);
    printf("学生信息已保存到文件！\n");
}

struct student *read_from_file(struct student *first)
{
FILE *fp = fopen("students.txt", "r");
if (!fp)
{
printf("文件打开失败！\n");
return first;
}

struct student *new_student, *last = NULL;
while (1)
{
new_student = (struct student *)malloc(sizeof(struct student));
if (!new_student)
{
printf("内存分配失败！\n");
fclose(fp);
return first;
}

if (fscanf(fp, "%ld %s %d %d %d %d %f", &new_student->studentID, new_student->name, 
&new_student->score[0], &new_student->score[1], &new_student->score[2], 
&new_student->sum, &new_student->average) != 7)
{
free(new_student);
break;
}

new_student->next = NULL;

if (first == NULL)
{
first = new_student;
last = new_student;
}
else
{
last->next = new_student;
last = new_student;
}
}

fclose(fp);
printf("学生信息已从文件读取！\n");
return first;
}

struct student *sort_students(struct student *first)
{
    if (first == NULL || first->next == NULL)
    {
        printf("学生信息不足两位无法进行排序！\n");
        return first;
    }

    struct student *i, *j;
    long temp_id;
    char temp_name[20];
    float temp_average;
    int temp_sum;

    for (i = first; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (i->sum < j->sum)
            {
                temp_id = i->studentID;
                i->studentID = j->studentID;
                j->studentID = temp_id;
                strcpy(temp_name, i->name);
                strcpy(i->name, j->name);
                strcpy(j->name, temp_name);
                for (int k = 0; k < 3; k++)
                {
                    int temp = i->score[k];
                    i->score[k] = j->score[k];
                    j->score[k] = temp;
                }
                temp_sum = i->sum;
                i->sum = j->sum;
                j->sum = temp_sum;
                temp_average = i->average;
                i->average = j->average;
                j->average = temp_average;
            }
        }
    }
    printf("学生信息已按总分进行排序！\n");
    return first;
}

void statistics_students(struct student *first)
{
    if (first == NULL)
    {
        printf("学生管理系统里没有学生信息可以统计！\n");
        return;
    }

    int total_scores[3] = {0, 0, 0}; 
    int student_count = 0;           
    float average_scores[3];         
    float highest_average = 0.0, lowest_average = 100.0; 
    int highest_subject = 0, lowest_subject = 0;         
    struct student *temp = first;

    while (temp != NULL)
    {
        for (int i = 0; i < 3; i++)
        {
            total_scores[i] += temp->score[i]; 
        }
        student_count++;
        temp = temp->next;
    }

    for (int i = 0; i < 3; i++)
    {
        average_scores[i] = (float)total_scores[i] / student_count;
        if (average_scores[i] > highest_average)
        {
            highest_average = average_scores[i];
            highest_subject = i + 1; 
        }
        if (average_scores[i] < lowest_average)
        {
            lowest_average = average_scores[i];
            lowest_subject = i + 1; 
        }
    }

    printf("学生管理系统统计结果如下:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("第 %d 门课程的平均分: %.2f\n", i + 1, average_scores[i]);
    }
    printf("最高平均分科目: 第 %d 门课程，平均分: %.2f\n", highest_subject, highest_average);
    printf("最低平均分科目: 第 %d 门课程，平均分: %.2f\n", lowest_subject, lowest_average);
}

void free_students(struct student **first)
{
    struct student *temp;
    while (*first != NULL)
    {
        temp = *first;
        *first = (*first)->next;
        free(temp);
    }
    *first = NULL; 
}

int main(void)
{
    struct student *first = NULL; 
    int enter;

    while (1)
    {
        menu();
        printf("请选择功能: ");
        scanf("%d", &enter);

        switch (enter)
        {
            case 1:
                first = add_students(first); 
                break;

            case 2:
                print_students(first);
                break;

            case 3:
                query_student(first);
                break;

            case 4:
                modify_student(first);
                break;

            case 5:
                first = delete_student(first);
                break;

            case 6:
                first = sort_students(first);
                break;

            case 7:
                statistics_students(first);
                break;

            case 8:
                save_to_file(first);
                break;

            case 9:
                first = read_from_file(first);
                break;

            case 10:
                printf("退出系统。\n");
                free_students(&first); 
                return 0;

            default:
                printf("输入错误！请选择有效的功能编号（1-10）。\n");
                break;
        }
    }
}
