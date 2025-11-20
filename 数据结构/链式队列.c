#include <stdio.h>
#include <stdlib.h>

typedef struct QNode {
    int data;
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front; // 指向队头结点
    QNode *rear;  // 指向队尾结点
    size_t size;
} LinkQueue;

void QueueInit(LinkQueue *q) {
    q->front = q->rear = NULL;
    q->size = 0;
}

int QueueEmpty(const LinkQueue *q) {
    return q->front == NULL;
}

int EnQueue(LinkQueue *q, int x) {
    QNode *n = (QNode *)malloc(sizeof(QNode));
    if (!n) return 0;
    n->data = x;
    n->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = n;
    } else {
        q->rear->next = n;
        q->rear = n;
    }
    q->size++;
    return 1;
}

int DeQueue(LinkQueue *q, int *out) {
    if (QueueEmpty(q)) return 0;
    QNode *n = q->front;
    if (out) *out = n->data;
    q->front = n->next;
    if (q->front == NULL) q->rear = NULL;
    free(n);
    q->size--;
    return 1;
}

void QueueClear(LinkQueue *q) {
    QNode *p = q->front;
    while (p) {
        QNode *n = p->next;
        free(p);
        p = n;
    }
    q->front = q->rear = NULL;
    q->size = 0;
}

static void QueuePrint(const LinkQueue *q) {
    printf("队头 -> ");
    for (QNode *p = q->front; p; p = p->next) {
        printf("%d ", p->data);
    }
    printf("-> 队尾\n");
}

int main(void) {
    LinkQueue q;
    QueueInit(&q);

    // 读入若干整数，0 结束
    printf("输入若干整数入队（0结束）：\n");
    while (1) {
        int x; if (scanf("%d", &x) != 1) break;
        if (x == 0) break;
        EnQueue(&q, x);
    }

    printf("当前队列：\n");
    QueuePrint(&q);

    printf("依次出队：\n");
    int x;
    while (DeQueue(&q, &x)) {
        printf("deq %d\n", x);
    }

    QueueClear(&q);
    return 0;
}
