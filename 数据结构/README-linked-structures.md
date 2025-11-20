# 链式栈与链式队列（对照练习范例）

包含两个可独立编译运行的 C 示例：
- `链式栈.c`：单链表实现的栈（push/pop/peek/clear）。
- `链式队列.c`：单链表实现的队列（enqueue/dequeue/clear）。

## 本地编译与运行（macOS zsh）
```sh
cd "/Users/chenrong/编程/my_project/数据结构"
# 链式栈
/opt/homebrew/bin/gcc-15 -std=c11 -O2 链式栈.c -o 链式栈 && ./链式栈
# 链式队列
/opt/homebrew/bin/gcc-15 -std=c11 -O2 链式队列.c -o 链式队列 && ./链式队列
```

## 代码要点
- 栈：`top` 指向栈顶结点，入栈头插、出栈头删，空栈判定 `top==NULL`。
- 队列：`front/rear` 分别指向队头/队尾，入队尾插、出队头删，空队判定 `front==NULL`。
- 所有 `malloc` 配对 `free`，`clear` 用于释放全部结点，避免内存泄漏。

## 推送到 GitHub（示例）
确保本地已配置远程 `origin`，然后：
```sh
cd "/Users/chenrong/编程/my_project"
 git checkout -b feat/linked-stack-queue
 git add 数据结构/链式栈.c 数据结构/链式队列.c 数据结构/README-linked-structures.md
 git commit -m "Add linked stack and linked queue examples with demos"
 git push -u origin feat/linked-stack-queue
```
随后在 GitHub 打开该分支创建 Pull Request。

如需改成 C++ 版本或抽出头文件（`.h`）做成可复用模块，告诉我即可。