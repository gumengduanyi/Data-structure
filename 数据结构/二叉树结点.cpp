// 二叉树结点示例程序（带中文逐行注释）
#include <stdio.h>
#include <stdlib.h>

// 定义二叉树节点结构体：值 + 左/右孩子指针
typedef struct TreeNode {
    int val;               // 节点存储的整数值
    struct TreeNode *l, *r; // 左子节点指针和右子节点指针
} TreeNode, *TreeNodePtr; // 同时定义类型别名 TreeNodePtr 为 TreeNode*

// 按先序输入建立二叉树（约定：输入整数，0 表示空节点）
// 例如输入序列：1 2 0 0 3 0 0 表示根 1，左子为 2（无子），右子为 3（无子）
TreeNode* build_preorder() {
    int x;
    // 读取一个整数到 x；如果读取失败（EOF）就返回 NULL
    if (scanf("%d", &x) != 1)
        return NULL;
    // 约定 0 表示空节点，不创建节点，直接返回 NULL
    if (x == 0)
        return NULL;
    // 为新节点分配内存并检查返回值
    TreeNode *n = (TreeNode*)malloc(sizeof(TreeNode));
    if (!n)
        return NULL; // 如果 malloc 失败，返回 NULL（调用者未做复杂错误处理）
    // 填充节点值，并递归构建左子树与右子树（先序输入对应的顺序）
    n->val = x;
    n->l = build_preorder();
    n->r = build_preorder();
    return n;
}

// 计算二叉树中结点总数（递归）
int countNodes(TreeNode *root) {
    if (!root) return 0; // 空树返回 0
    // 当前节点 + 左子树结点数 + 右子树结点数
    return 1 + countNodes(root->l) + countNodes(root->r);
}

// 计算叶子结点数（递归）：没有左也没有右的节点算作叶子
int countLeaves(TreeNode *root) {
    if (!root) return 0;
    if (!root->l && !root->r) return 1; // 自身为叶子
    return countLeaves(root->l) + countLeaves(root->r);
}

// 计算二叉树深度（递归）：空树深度为 0，单节点深度为 1
int treeDepth(TreeNode *root) {
    if (!root) return 0;
    int dl = treeDepth(root->l);
    int dr = treeDepth(root->r);
    // 返回左右子树中较大深度 + 1（包括当前根）
    return (dl > dr ? dl : dr) + 1;
}

// 非递归中序遍历：使用显式栈（手工管理指针数组）
// 中序遍历顺序为：左 -> 根 -> 右
void inorder_iterative(TreeNode *root) {
    // 初始栈容量（用于存放 TreeNode*）
    int cap = 64;
    // 在堆上分配一个指针数组作为栈：每个元素存放一个 TreeNodePtr (即 TreeNode*)
    TreeNodePtr *stack = (TreeNodePtr*)malloc(sizeof(TreeNodePtr) * cap);
    // top 表示数组中下一个可写位置，也即当前栈大小
    int top = 0;
    // cur 指向当前遍历到的节点（起始为根）
    TreeNode *cur = root;

    // 当还有当前节点要处理，或栈中仍有未访问的节点时循环
    while (cur != NULL || top > 0) {
        // 先把当前节点以及它的一系列左孩子全部压入栈
        // 这确保最左边的节点最先被访问（中序的 '左' 步骤）
        while (cur != NULL) {
            // 如果数组已满，按倍增策略扩容
            if (top >= cap) {
                cap *= 2;
                // 注意：此处直接把 realloc 的返回值赋回 stack，实际生产
                // 代码中建议先用临时指针检查 realloc 是否返回 NULL
                stack = (TreeNodePtr*)realloc(stack, sizeof(TreeNodePtr) * cap);
            }
            // 将当前节点指针压入栈顶
            stack[top++] = cur;
            // 继续向左走
            cur = cur->l;
        }
        // 当没有更左的节点时，从栈顶弹出上一个节点并访问它（中序中的 '根'）
        cur = stack[--top];
        printf("%d ", cur->val); // 访问动作：打印节点值
        // 然后转向该节点的右子树，下一轮循环将处理右子树的左路径
        cur = cur->r;
    }
    // 遍历结束，释放栈的内存
    free(stack);
}

// 递归释放二叉树占用的内存（后序释放）
void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->l);
    free_tree(root->r);
    free(root);
}

int main() {
    // 提示用户输入（或通过重定向/管道传入数据）
    printf("请输入先序序列（0 表示空），例如：1 2 0 0 3 0 0\n");
    TreeNode *root = build_preorder();
    // 计算并打印各种度量
    int nodes = countNodes(root);
    int leaves = countLeaves(root);
    int depth = treeDepth(root);
    printf("结点总数: %d\n", nodes);
    printf("叶子结点数: %d\n", leaves);
    printf("树的深度: %d\n", depth);
    printf("非递归中序遍历：");
    inorder_iterative(root);
    printf("\n");
    // 释放内存并退出
    free_tree(root);
    return 0;
}
