#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    int data[MAXSIZE];
    int front, rear;
} Queue;

typedef struct {
    int data[MAXSIZE];
    int top;
} Stack;

// 队列操作
void initQueue(Queue *q) { q->front = q->rear = 0; }
int isEmptyQueue(Queue *q) { return q->front == q->rear; }
void enqueue(Queue *q, int x) { q->data[q->rear++] = x; }
int dequeue(Queue *q) { return q->data[q->front++]; }

// 栈操作
void initStack(Stack *s) { s->top = -1; }
int isEmptyStack(Stack *s) { return s->top == -1; }
void push(Stack *s, int x) { s->data[++s->top] = x; }
int pop(Stack *s) { return s->data[s->top--]; }

// 队列逆置
void reverseQueue(Queue *q) {
    Stack s;
    initStack(&s);
    // 队列元素全部入栈
    while (!isEmptyQueue(q)) {
        push(&s, dequeue(q));
    }
    // 栈元素全部入队
    initQueue(q); // 清空队列
    while (!isEmptyStack(&s)) {
        enqueue(q, pop(&s));
    }
}

int main() {
    Queue q;
    initQueue(&q);
    // 示例：入队 1,2,3,4,5
    for (int i = 1; i <= 5; i++) enqueue(&q, i);

    reverseQueue(&q);

    // 输出逆置后的队列
    while (!isEmptyQueue(&q)) {
        printf("%d ", dequeue(&q));
    }
    return 0;
}