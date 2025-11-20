#include <stdio.h>
#include <string.h>
#define M 50  // 最多50个学生
#define N 3   // 3门科目

// 学生结构体
typedef struct 
{
    char xh[12];        // 学号
    char name[20];      // 姓名
    float scores[N];    // 三科成绩
    float sum;        // 总分
    float average;      // 平均分
} Student;

Student students[M];    // 学生数组
int Count = 0;   // 当前学生数量

// 显示菜单
void Menu() 
{
    printf("\n===== 学生成绩管理系统 =====\n");
    printf("1. 添加学生信息\n");
    printf("2. 显示所有学生信息\n");
    printf("3. 按学号查询学生\n");
    printf("4. 按学号修改成绩\n");
    printf("5. 按学号删除学生\n");
    printf("6. 显示优秀学生(平均分>80)\n");
    printf("7. 按平均成绩排序\n");
    printf("8. 退出系统\n");
    printf("============================\n");
}

// 输入学生数据
void input(Student *s) 
{
    printf("请输入学号: ");
    scanf("%s", (*s).xh);// 输入学号
    
    printf("请输入姓名: ");
    scanf("%s", (*s).name);// 输入姓名
    
    (*s).sum= 0;
    for (int i = 0; i < N; i++) 
	{
        printf("请输入%s的第%d门成绩: ",(*s).name, i+1);
        scanf("%f", &(*s).scores[i]);// 输入每科成绩
        (*s).sum += (*s).scores[i];// 累加总分
    }
    (*s).average = (*s).sum / N;// 计算平均分
}

// 添加学生
void add() 
{
    if (Count >= M) 
	{
        printf("学生数量已达上限！\n");
        return;
    }
    
    Student s;
    input(&s);// 输入数据
    
    // 检查学号是否已存在
    for (int i = 0; i < Count; i++) 
	{
        if (strcmp(students[i].xh, s.xh) == 0) 
		{
            printf("该学号已存在！\n");
            return;
        }
    }
    
    students[Count] = s;// 存入数组
    Count++;// 学生数+1
    printf("添加成功！\n");
}

// 显示学生表头
void displayStudentHeader() 
{
    printf("\n学号\t姓名\t科目1\t科目2\t科目3\t总分\t平均分\n ");
    printf("------------------------------------------------------------\n");
}

// 显示单个学生信息
void displayStudent(const Student *s) //const 是一个类型限定符
{
    printf("%s\t%s\t", (*s).xh, (*s).name);// 打印学号姓名
    for (int j = 0; j < N; j++) 
	{
        printf("%.1f\t", (*s).scores[j]);// 打印各科成绩
    }
    printf("%.1f\t%.1f\n", (*s).sum, (*s).average);// 打印总分均分
}

// 显示所有学生
void showAllStudents() 
{
    if (Count == 0) 
	{
        printf("没有学生记录！\n");
        return;
    }
    
    displayStudentHeader();
    for (int i = 0; i < Count; i++) 
	{
        displayStudent(&students[i]);
    }
}

// 按学号查找学生索引
int findIndex(const char* xh) 
{
    for (int i = 0; i < Count; i++) 
	{
        if (strcmp(students[i].xh, xh) == 0)
		{
            return i; // 返回找到的索引
        }
    }
    return -1; // 未找到
}

// 查询学生
void search() 
{
    char xh[12];
    printf("\n请输入要查询的学号: ");
    scanf("%s", xh);
    
    int index = findIndex(xh);
    if (index == -1) 
	{
        printf("未找到该学生！\n");
        return;
    }
    
    printf("\n查询结果:\n");
    displayStudentHeader();
    displayStudent(&students[index]);
}

// 修改成绩
void modify() 
{
    char xh[12];
    printf("\n请输入要修改的学号: ");
    scanf("%s", xh);
    
    int index = findIndex(xh);
    if (index == -1) 
	{
        printf("未找到该学生！\n");
        return;
    }
    
    printf("\n当前学生信息:\n");
    displayStudentHeader();
    displayStudent(&students[index]);
    
    printf("\n请输入新的成绩:\n");
    students[index].sum = 0;
    for (int i = 0; i < N; i++) 
	{
        printf("科目%d: ", i+1);
        scanf("%f", &students[index].scores[i]);
        students[index].sum += students[index].scores[i];
    }
    students[index].average = students[index].sum / N;
    
    printf("\n修改后的信息:\n");
    displayStudentHeader();
    displayStudent(&students[index]);
}

// 删除学生信息
void removeStudent() 
{
    char xh[12];
    printf("\n请输入要删除的学号: ");
    scanf("%s", xh);
    
    int index = findIndex(xh);
    if (index == -1)
	{
        printf("未找到该学生！\n");
        return;
    }
    
    // 将后面的学生前移
    for (int i = index; i < Count-1; i++) 
	{
        students[i] = students[i+1];
    }
    Count--;
    
    printf("删除成功！\n");
}

// 显示优秀学生
void showExcellentStudents() 
{
    if (Count == 0) 
	{
        printf("没有学生记录！\n");
        return;
    }
    
    printf("\n优秀学生(平均分>80):\n");
    displayStudentHeader();
    
    int found = 0;
    for (int i = 0; i < Count; i++) 
	{
        if (students[i].average > 80) 
		{
            displayStudent(&students[i]);
            found = 1;
        }
    }
    
    if (!found) 
	{
        printf("没有优秀学生！\n");
    }
}

// 按平均成绩排序
void sort() 
{
    if (Count == 0) 
	{
        printf("没有学生记录！\n");
        return;
    }
    
    // 创建临时数组用于排序
    Student temp[M];
    for (int i = 0; i < Count; i++) 
	{
        temp[i] = students[i];// 复制数据
    }
    
    // 冒泡排序 - 按平均成绩降序排列
    for (int i = 0; i < Count-1; i++) 
	{
        for (int j = 0; j < Count-i-1; j++) 
		{
            if (temp[j].average < temp[j+1].average) 
			{
                Student t = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = t;
            }
        }
    }
    
    // 显示排序结果
    printf("\n按平均成绩排序结果:\n");
    printf("\n排名\t学号\t姓名\t科目1\t科目2\t科目3\t总分\t平均分\n ");
    printf("------------------------------------------------------------\n");

    
    for (int i = 0; i < Count; i++) 
	{
        printf("%d\t", i+1);
        displayStudent(&temp[i]);
    }
}

int main() 
{
    int choice;
    
    do 
	{
        Menu();
        printf("请选择操作(1-8): ");
        scanf("%d", &choice);
        
        switch(choice) 
		{
            case 1: add(); break;
            case 2: showAllStudents(); break;
            case 3: search(); break;
            case 4: modify(); break;
            case 5: removeStudent(); break;
            case 6: showExcellentStudents(); break;
            case 7: sort(); break;
            case 8: printf("感谢使用，再见！\n"); break;
            default: printf("无效选择，请重新输入！\n");
        }
    } 
	while(choice != 8);
    
    return 0;
}