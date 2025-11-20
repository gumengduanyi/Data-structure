#include "stdio.h"
#include "stdlib.h"

#define MAXQSIZE 100                 //最大队列长度
#define MAXSTACKSIZE 100

typedef struct{
    int *base;                      //初始化的动态分配存储空间
    int front;                      //队头指针
    int rear;                       //队尾指针
}SqQueue;                           //循环队列

typedef struct{
    int data[MAXSTACKSIZE];
    int top;
}SqStack;

int InitQueue(SqQueue &Q){          //构造一个空队列
    Q.base = (int *)malloc(MAXQSIZE * sizeof(int));
    if(!Q.base){
        printf("Error\n");
        return -1;
    }
    Q.front = Q.rear = 0;
    return 1;
} //InitQueue

int QueueEmpty(SqQueue Q) {          //判断队列是否为空
    if(Q.rear == Q.front) {
        printf("★队列空★\n");
        return 1;  // 1表示空
    } else {
        printf("★★队列不空★\n");
        return 0;  // 0表示不空
    }
} //QueueEmpty

int EnQueue(SqQueue &Q, int &e){    //元素入队列
    if(((Q.rear+1) % MAXQSIZE) == Q.front){
        printf("Error\n");
        return -1;
    }
    Q.base[Q.rear] = e;
    Q.rear = (Q.rear+1) % MAXQSIZE;
    return 1;
} //EnQueue

int DeQueue(SqQueue &Q, int &e){    //元素出队列，值用e带回
    if(Q.front == Q.rear){
        printf("队列为空，无法出队！\n");
        return -1;
    }
    e = Q.base[Q.front];
    Q.front = (Q.front+1)%MAXQSIZE;
    return 1;
} //DeQueue

void printQueue(SqQueue *q) {
    printf("队列内容：");
    int i = q->front;
    while (i != q->rear) {
        printf("%d ", q->base[i]);
        i = (i + 1) % MAXQSIZE;
    }
    printf("\n");
}

// 栈操作
void InitStack(SqStack *S) { S->top = -1; }
int StackEmpty(SqStack *S) { return S->top == -1; }
int Push(SqStack *S, int e) {
    if(S->top >= MAXSTACKSIZE-1) return 0;
    S->data[++S->top] = e;
    return 1;
}
int Pop(SqStack *S, int *e) {
    if(StackEmpty(S)) return 0;
    *e = S->data[S->top--];
    return 1;
}

// 利用栈逆置队列
void ReverseQueue(SqQueue *Q) {
    SqStack S;
    InitStack(&S);
    int x;
    int count = 0;
    // 队列元素全部入栈
    while (Q->front != Q->rear) {
        DeQueue(*Q, x);
        Push(&S, x);
        count++;
    }
    // 只重置 front、rear 指针，不重新分配空间
    Q->front = 0;
    Q->rear = 0;
    // 栈元素全部入队
    while (!StackEmpty(&S)) {
        Pop(&S, &x);
        Q->base[Q->rear] = x;
        Q->rear = (Q->rear + 1) % MAXQSIZE;
    }
}

void queueManage() {
    SqQueue q;
    InitQueue(q);
    int num;
    while (1) {
        printf("请输入一个整数（0结束）：");
        scanf("%d", &num);
        if (num == 0) break;
        if (num % 2 != 0) { // 奇数入队
            EnQueue(q, num);
            printf("奇数 %d 入队。\n", num);
        } else { // 偶数时，队头元素出队
            int x;
            if (q.front != q.rear) { // 直接检查队列是否为空，不调用QueueEmpty
                DeQueue(q, x);
                printf("队头元素 %d 出队。\n", x);
            } else {
                printf("队列已空，无法出队。\n");
            }
        }
        printQueue(&q); // 添加这行来显示当前队列状态
    }
    printf("算法结束。\n");
}

int main(){
    SqQueue Q;
    int e,order;
    char yes_no;

    printf("\t\t          菜   单\n");
    printf("\t\t※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※ ※\n");
    printf("\t\t---------------------------------------------\n");
    printf("\t\t1. 初始化队列\n");
    printf("\t\t---------------------------------------------\n");
    printf("\t\t2. 检查队列是否为空\n");
    printf("\t\t---------------------------------------------\n");
    printf("\t\t3. 入队元素\n");
    printf("\t\t---------------------------------------------\n");
    printf("\t\t4. 出队元素并显示全部\n");
    printf("\t\t---------------------------------------------\n");
    printf("\t\t5. 利用栈逆置队列\n");
    printf("\t\t---------------------------------------------\n");
    printf("\t\t6. 退出\n");
    printf("\t\t---------------------------------------------\n");
    printf("\t\t7. 队列模拟管理算法\n");
    printf("\t\t---------------------------------------------\n\n\n\n\n");

    do{
        printf("Please enter your choice:");
        scanf("%d",&order);
        printf("\n");
        switch(order){
            case 1:
                InitQueue(Q);
                break;
            case 2:
                QueueEmpty(Q);
                break;
            case 3:
                do{
                    printf("\n请输入入队列的元素的值e:\n");
                    scanf("%d",&e);
                    EnQueue(Q,e);
                    printf("\n要继续选择吗(Y/N)?");
                    do{
                        yes_no = getchar();
                    }while(yes_no!='Y' && yes_no!='y' && yes_no!='N' && yes_no!='n');
                }while(yes_no!='N' && yes_no!='n');
                break;
            case 4:
                printf("\n队列中元素为:\n");
                printQueue(&Q);
                while(Q.front != Q.rear){
                    DeQueue(Q,e);
                    printf("%d\t",e);
                }
                printf("\n");
                break;
            case 5:
                printf("\n逆置前：");
                printQueue(&Q);
                ReverseQueue(&Q);
                printf("逆置后：");
                printQueue(&Q);
                break;
            case 6:
                return 0;
            case 7:
                queueManage();
                break;
            default:
                printf("Error\n");
        }
        if(order==0){
            break;
        }
        printf("\n\n\n\n\n---------------------------------------------\n");
        printf("要继续选择吗(Y/N)?");
        do{
            yes_no=getchar();
        }while(yes_no!='Y'&&yes_no!='y'&&yes_no!='N'&&yes_no!='n');
    }while(yes_no!='N'&&yes_no!='n');

    return 0;
}