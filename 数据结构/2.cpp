#include <stdio.h>
#include <stdlib.h>

#define MAXQSIZE 100
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef struct {
    int *base;
    int *top;
    int stacksize;
} SqStack;

typedef struct {
    int *base;
    int front;
    int rear;
} SqQueue;

int InitStack(SqStack &S){
    //初始化栈
    S.base = (int *)malloc(STACK_INIT_SIZE * sizeof(int));
    if(!S.base){
        return -1; //存储分配失败
    }
    S.top = S.base;
    S.stacksize = STACK_INIT_SIZE;
    return 1;
}

int StackEmpty(SqStack S){
    //判断栈是否为空
    if(S.top == S.base){
        return 1; //栈空
    }
    else{
        return 0; //栈非空
    }
}

int Push(SqStack &S, int e){
    //进栈
    if(S.top - S.base >= S.stacksize){
        S.base = (int *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(int));
        if(!S.base){
            printf("Error\n");
            return -1; //存储分配失败
        }
        S.top = S.base + S.stacksize;
        S.stacksize += STACKINCREMENT;
    }
    *(S.top) = e;
    S.top++;
    return 1;
}

int Pop(SqStack &S, int &e){
    //出栈
    if(S.top == S.base){
        printf("Error\n");
        return -1; //栈空
    }
    S.top--;
    e = *(S.top);
    // e = *(--S.top);
    return 1;
}
/*void Conversion(SqStack &S){
    //十进制转为二进制
    int N, e;
    InitStack(S);
    printf("请输入一个非负十进制整数:");
    scanf("%d", &N);
    while(N){
        Push(S, N % 2);
        N = N / 2;
    }
    printf("\n对应二进制数为:");
    while(!StackEmpty(S)){
        Pop(S, e);
        printf("%d", e);
    }
}
    */

//十进制转八进制
/*void Conversion(SqStack &S){
    //十进制转为八进制
    int N, e;
    InitStack(S);
    printf("请输入一个非负十进制整数:");
    scanf("%d", &N);
    while(N){
        Push(S, N % 8);
        N = N / 8;
    }
    printf("\n对应八进制数为:");
    while(!StackEmpty(S)){
        Pop(S, e);
        printf("%d", e);
    }
}
*/

//十进制转十六进制
void Conversion(SqStack &S){
    //十进制转为十六进制
    int N, e;
    InitStack(S);
    printf("请输入一个非负十进制整数:");
    scanf("%d", &N);
    while(N){
        Push(S, N % 16);
        N = N / 16;
    }
    printf("\n对应十六进制数为:");
     while(!StackEmpty(S)){
        Pop(S, e);
        printf("%d", e);
    }
}


int main(){
    SqStack S;
    SqQueue Q;
    int a, e, order;
    char yes_no;

    printf("\t\t           菜   单\n");
    printf("\t\t※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※\n");
    printf("\t\t-----------------------------\n");
    printf("\t\t1.Init a stack\n");
    printf("\t\t-----------------------------\n");
    printf("\t\t2.Check whether the stack is empty\n");
    printf("\t\t-----------------------------\n");
    printf("\t\t3.Push the stack\n");
    printf("\t\t-----------------------------\n");
    printf("\t\t4.Delete the stack\n");
    printf("\t\t-----------------------------\n");
    printf("\t\t5.Tranform integer to binary integer\n");
    printf("\t\t-----------------------------\n");

    do{
        printf("Please enter your choice:");
        scanf("%d", &order);
        switch(order){
            case 1: 
                    InitStack(S); 
                    break;
            case 2: 
                    a = StackEmpty(S);
                    if(a){
                        printf("The stack is empty\n");
                    }
                    else{
                        printf("The stack is not empty\n");
                    }
                    break;
            case 3: 
                    printf("Please enter an integer to push into the stack:");
                    scanf("%d", &e);
                    Push(S, e); 
                    break;
            case 4: 
                    Pop(S, e); 
                    printf("The deleted element is %d\n", e);
                    break;
            case 5:
                    Conversion(S);
                    break;
            default: 
                    printf("err\n"); 
        }//switch
        printf("\n-----------------------------\n");
        printf("Do you want to continue(Y/N)?");
        do{
            yes_no = getchar();
        }while(yes_no == '\n');
        printf("\n-----------------------------\n\n");
    }while(yes_no == 'Y' || yes_no == 'y');
}
