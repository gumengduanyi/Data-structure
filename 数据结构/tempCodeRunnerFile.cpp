int treeDepth(TreeNode *root) {
    if (!root) return 0;
    int dl = treeDepth(root->l);
    int dr = treeDepth(root->r);
    // 返回左右子树中较大深度 + 1（包括当前根）
    return (dl > dr ? dl : dr) + 1;
}