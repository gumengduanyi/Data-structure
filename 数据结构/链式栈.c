#include <stdio.h>
#include <stdlib.h>

typedef struct StackNode {
    int data;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    size_t size;
} LinkStack;

void StackInit(LinkStack *s) {
    s->top = NULL;
    s->size = 0;
}

int StackEmpty(const LinkStack *s) {
    return s->top == NULL;
}

int StackPush(LinkStack *s, int x) {
    StackNode *n = (StackNode *)malloc(sizeof(StackNode));
    if (!n) return 0;
    n->data = x;
    n->next = s->top;
    s->top = n;
    s->size++;
    return 1;
}

int StackPop(LinkStack *s, int *out) {
    if (StackEmpty(s)) return 0;
    StackNode *t = s->top;
    if (out) *out = t->data;
    s->top = t->next;
    free(t);
    s->size--;
    return 1;
}

int StackPeek(const LinkStack *s, int *out) {
    if (StackEmpty(s)) return 0;
    if (out) *out = s->top->data;
    return 1;
}

void StackClear(LinkStack *s) {
    StackNode *p = s->top;
    while (p) {
        StackNode *n = p->next;
        free(p);
        p = n;
    }
    s->top = NULL;
    s->size = 0;
}

static void StackPrint(LinkStack *s) {
    printf("栈顶 -> ");
    for (StackNode *p = s->top; p; p = p->next) {
        printf("%d ", p->data);
    }
    printf("-> 底\n");
}

int main(void) {
    LinkStack s;
    StackInit(&s);

    // 示例：压入 1..5
    for (int i = 1; i <= 5; ++i) {
        StackPush(&s, i);
    }
    printf("初始栈：\n");
    StackPrint(&s);

    int x;
    printf("依次出栈：\n");
    while (StackPop(&s, &x)) {
        printf("pop %d\n", x);
    }

    StackClear(&s);
    return 0;
}
