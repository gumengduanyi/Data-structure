// 二叉树结点示例：使用 size_t 表示容量与栈索引，并对 realloc 做检查
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *l, *r;
} TreeNode, *TreeNodePtr;

TreeNode* build_preorder() {
    int x;
    if (scanf("%d", &x) != 1) return NULL;
    if (x == 0) return NULL;
    TreeNode *n = (TreeNode*)malloc(sizeof(TreeNode));
    if (!n) return NULL;
    n->val = x;
    n->l = build_preorder();
    n->r = build_preorder();
    return n;
}

int countNodes(TreeNode *root) {
    if (!root) return 0;
    return 1 + countNodes(root->l) + countNodes(root->r);
}

int countLeaves(TreeNode *root) {
    if (!root) return 0;
    if (!root->l && !root->r) return 1;
    return countLeaves(root->l) + countLeaves(root->r);
}

int treeDepth(TreeNode *root) {
    if (!root) return 0;
    int dl = treeDepth(root->l);
    int dr = treeDepth(root->r);
    return (dl > dr ? dl : dr) + 1;
}

void inorder_iterative(TreeNode *root) {
    // 使用 size_t 表示容量和栈索引
    size_t cap = 64;
    TreeNodePtr *stack = (TreeNodePtr*)malloc(sizeof *stack * cap);
    if (!stack) {
        perror("malloc");
        return;
    }
    size_t top = 0; // top 表示下一个可写索引（栈大小）
    TreeNode *cur = root;

    while (cur != NULL || top > 0) {
        while (cur != NULL) {
            if (top >= cap) {
                size_t newcap = cap * 2;
                TreeNodePtr *tmp = (TreeNodePtr*)realloc(stack, sizeof *stack * newcap);
                if (!tmp) {
                    // realloc 失败，释放原内存并中止遍历
                    free(stack);
                    perror("realloc");
                    return;
                }
                stack = tmp;
                cap = newcap;
            }
            stack[top++] = cur;
            cur = cur->l;
        }
        cur = stack[--top];
        printf("%d ", cur->val);
        cur = cur->r;
    }
    free(stack);
}

void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->l);
    free_tree(root->r);
    free(root);
}

int main() {
    printf("请输入先序序列（0 表示空），例如：1 2 0 0 3 0 0\n");
    TreeNode *root = build_preorder();
    int nodes = countNodes(root);
    int leaves = countLeaves(root);
    int depth = treeDepth(root);
    printf("结点总数: %d\n", nodes);
    printf("叶子结点数: %d\n", leaves);
    printf("树的深度: %d\n", depth);
    printf("非递归中序遍历（size_t 版）：");
    inorder_iterative(root);
    printf("\n");
    free_tree(root);
    return 0;
}
