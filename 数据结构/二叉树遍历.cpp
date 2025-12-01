#include <stdio.h>
#include <stdlib.h>

/*
    二叉树遍历示例程序
    - 按先序序列从 stdin 构建二叉树（约定：用 0 表示空节点）
    - 打印前序/中序/后序/层序遍历结果
    - 释放树的内存

    关键点：输入格式是先序（preorder）：
        对于每个节点，输入顺序为：根 节点的先序序列(左子树) 节点的先序序列(右子树)
        用 `0` 表示某个子树为空。
    举例：`1 2 0 0 3 0 0` 表示根 1，左子 2（左右为空），右子 3（左右为空）。
*/

typedef struct TreeNode {
        int val;              // 节点保存的整数值
        struct TreeNode *l, *r; // 左/右子指针
} TreeNode;

/*
  build_preorder
  - 从 stdin 读取下一个整数。
  - 如果读取失败（scanf != 1）或到达 EOF，则返回 NULL（停止构建）。
  - 如果读到 0，表示该位置为空子树，返回 NULL。
  - 否则为当前值创建节点，递归构建左子树和右子树（先序构建）。

  注意：
  - scanf 返回 1 表示成功读取一个整数；返回 0 表示当前字符不是数字（不会消费该字符）；返回 EOF 表示输入结束或错误。
  - 输入必须包含恰当的占位（0）来标明空节点，否则会出现偏斜（例如全部作为左子树）。
  - 如果希望更稳健地处理错误输入，可改为用 fgets + strtol 逐字段解析。
*/
TreeNode* build_preorder() {
    int x;
    if (scanf("%d", &x) != 1)
        return NULL; // 输入结束或格式错误 -> 视为无节点
    if (x == 0)
        return NULL; // 0 表示空节点
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = x;
    node->l = build_preorder(); // 递归构建左子树（消耗后续输入）
    node->r = build_preorder(); // 然后构建右子树
    return node;
}

/*
    递归前序遍历（根 左 右）
    时间复杂度 O(n)，空间复杂度由递归栈决定 O(h)，h 为树高。
*/
void preorder(TreeNode *root) {
        if (!root) return;
        printf("%d ", root->val);
        preorder(root->l);
        preorder(root->r);
}

/*
    递归中序遍历（左 根 右）
*/
void inorder(TreeNode *root) {
        if (!root) return;
        inorder(root->l);
        printf("%d ", root->val);
        inorder(root->r);
}

/*
    递归后序遍历（左 右 根）
*/
void postorder(TreeNode *root) {
        if (!root) return;
        postorder(root->l);
        postorder(root->r);
        printf("%d ", root->val);
}

/*
  简单循环队列（只用于层序遍历示例）
  - cap 为容量，采用 head/tail 指针循环使用数组空间
  - 本实现未检测“队列满”的情况，假定 cap 足够大；生产代码应检查并在需要时扩容或返回错误
*/
typedef struct {
    TreeNode **data;
    int head, tail, cap;
} TQueue;

TQueue* q_create(int cap) {
    TQueue *q = (TQueue*)malloc(sizeof(TQueue));
    q->data = (TreeNode**)malloc(sizeof(TreeNode*) * cap);
    q->head = q->tail = 0; q->cap = cap;
    return q;
}
int q_empty(TQueue *q) { return q->head == q->tail; }
int q_push(TQueue *q, TreeNode *n) { q->data[q->tail++] = n; if (q->tail==q->cap) q->tail=0; return 1; }
TreeNode* q_pop(TQueue *q) { TreeNode *t = q->data[q->head++]; if (q->head==q->cap) q->head=0; return t; }
void q_free(TQueue *q) { free(q->data); free(q); }

/*
  层序遍历（广度优先）
  - 使用循环队列按层访问节点，时间 O(n)，空间 O(n)（队列）
  - 注意：如果树非常大或 cap 太小，q_push 会越界（本示例 cap=1024），可改为动态扩容
*/
void levelorder(TreeNode *root) {
    if (!root) return;
    TQueue *q = q_create(1024);
    q_push(q, root);
    while (!q_empty(q)) {
        TreeNode *t = q_pop(q);
        printf("%d ", t->val);
        if (t->l) q_push(q, t->l);
        if (t->r) q_push(q, t->r);
    }
    printf("\n");
    q_free(q);
}

/*
    释放树的内存（后序释放）
*/
void free_tree(TreeNode *root) {
        if (!root) return;
        free_tree(root->l);
        free_tree(root->r);
        free(root);
}

int main() {
    printf("请输入先序序列（用空节点0表示），以空格分隔并以回车结束，例如：1 2 0 0 3 0 0\n");
    /*
      运行说明：在交互模式下输入先序序列并回车；或用管道传入序列，例如：
        echo "1 2 0 0 3 0 0" | ./二叉树遍历
    */
    TreeNode *root = build_preorder();
    printf("前序遍历："); preorder(root); printf("\n");
    printf("中序遍历："); inorder(root); printf("\n");
    printf("后序遍历："); postorder(root); printf("\n");
    printf("层序遍历："); levelorder(root);
    free_tree(root);
    return 0;
}
