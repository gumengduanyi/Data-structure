/*
 * 简单的哈夫曼编码/解码（C 实现）
 * 用法：
 *   压缩: ./huffman -c input.txt output.huf
 *   解压: ./huffman -d input.huf output.txt
 *
 * 实现细节：
 * - 先读取整个输入文件，统计 256 字节频率（uint32_t）
 * - 输出文件头：原始字节数 (uint32_t) + 256 x uint32_t 频率表
 * - 构建哈夫曼树（以线性扫描最小频率为简洁实现）
 * - 生成码表，用字符串 '0'/'1' 表示码（用于编码阶段）
 * - 编码时按位打包为字节写出；结尾不足一字节用 0 填充
 * - 解码时读频率表重建树，根据总字节数停止输出
 *
 * 这是教学/练习实现，非生产级：对大文件内存占用较高，但逻辑清晰。
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct Node {
    unsigned char ch;     // 只有叶子有效
    uint32_t freq;
    struct Node *left, *right;
} Node;

// 创建叶子或内部节点
static Node* node_new(unsigned char ch, uint32_t freq, Node* l, Node* r) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) return NULL;
    n->ch = ch;
    n->freq = freq;
    n->left = l;
    n->right = r;
    return n;
}

// 释放哈夫曼树
static void node_free(Node *r) {
    if (!r) return;
    node_free(r->left);
    node_free(r->right);
    free(r);
}

// 找到频率最小的两个节点索引（线性扫描简洁实现）
static void pick_two_min(Node **arr, int n, int *i1, int *i2) {
    int a=-1, b=-1;
    for (int i=0;i<n;i++) {
        if (!arr[i]) 
        continue;
        if (a==-1 || arr[i]->freq < arr[a]->freq) 
        { 
            b=a; 
            a=i; 
        }
        else if (b==-1 || arr[i]->freq < arr[b]->freq) 
        { 
            b=i; 
        }
    }
    *i1 = a; 
    *i2 = b;
}

// 递归生成码表；codes[i] 为动态分配字符串（'0'/'1' 结尾 '\0'）
static void build_codes(Node *r, char **codes, char *buf, int depth) {
    if (!r) return;
    if (!r->left && !r->right) {
        // 叶子
        buf[depth] = '\0';
        codes[(unsigned char)r->ch] = strdup(buf);
        return;
    }
    if (r->left) {
        buf[depth] = '0';
        build_codes(r->left, codes, buf, depth+1);
    }
    if (r->right) {
        buf[depth] = '1';
        build_codes(r->right, codes, buf, depth+1);
    }
}

// 压缩主流程
static int compress_file(const char *inpath, const char *outpath) {
    FILE *in = fopen(inpath, "rb");
    if (!in) 
    { 
        perror("fopen input"); 
        return 1; 
    }
    // 读取全部数据到内存（教学实现）
    if (fseek(in, 0, SEEK_END) != 0) 
    { 
        perror("fseek"); 
        fclose(in); 
        return 1; 
    }
    long len = ftell(in);
    if (len < 0) 
    { 
        perror("ftell"); 
        fclose(in); 
        return 1; 
    }
    rewind(in);
    unsigned char *buf = (unsigned char*)malloc(len>0?len:1);
    if (!buf && len>0) 
    { 
        perror("malloc"); 
        fclose(in); 
        return 1; 
    }
    size_t readn = fread(buf, 1, len, in);
    fclose(in);

    // 统计频率
    uint32_t freq[256] = {0};
    for (size_t i=0;i<readn;i++) freq[buf[i]]++;

    // 构建节点数组（非空频率）
    Node *nodes[512]; int n_nodes = 0;
    for (int c=0;c<256;c++) {
        if (freq[c] > 0) {
            nodes[n_nodes++] = node_new((unsigned char)c, freq[c], NULL, NULL);
        }
    }
    // 特殊情况：如果输入只有 0 字节（空文件）或只有一种字符
    if (readn == 0) {
        // 写空文件头（原始长度 0，频率全 0），没有数据
        FILE *out = fopen(outpath, "wb");
        if (!out) { perror("fopen output"); free(buf); return 1; }
        uint32_t orig = 0;
        fwrite(&orig, sizeof(orig), 1, out);
        uint32_t zero = 0;
        for (int i=0;i<256;i++) fwrite(&zero, sizeof(zero), 1, out);
        fclose(out);
        free(buf);
        return 0;
    }
    if (n_nodes == 1) {
        // 只有一种字符：构造一个根和左叶（或右叶）保证解码时有树
        Node *only = nodes[0];
        Node *root = node_new(0, only->freq, only, NULL);
        nodes[0] = root; n_nodes = 1;
        // 继续用下面的流程（root 在 nodes[0]）
    } else {
        // 合并最小的两个节点直到只剩一个
        while (n_nodes > 1) {
            int i1=-1, i2=-1;
            pick_two_min(nodes, n_nodes, &i1, &i2);
            if (i1==-1 || i2==-1) break;
            Node *a = nodes[i1];
            Node *b = nodes[i2];
            Node *p = node_new(0, a->freq + b->freq, a, b);
            // 把 p 放在 i1 位置，移除 i2
            nodes[i1] = p;
            // remove i2 by swapping last into i2
            nodes[i2] = nodes[n_nodes-1];
            n_nodes--;
        }
    }
    Node *root = nodes[0];

    // 生成码表
    char *codes[256] = {0};
    char tmpbuf[512];
    build_codes(root, codes, tmpbuf, 0);

    // 打开输出文件并写 header: 原始长度 + 256*uint32_t 频率表
    FILE *out = fopen(outpath, "wb");
    if (!out) { perror("fopen output"); free(buf); node_free(root); return 1; }
    uint32_t orig_size = (uint32_t)readn;
    fwrite(&orig_size, sizeof(orig_size), 1, out);
    for (int i=0;i<256;i++) fwrite(&freq[i], sizeof(freq[i]), 1, out);

    // 按位写入数据
    unsigned char outbyte = 0; int outbit = 0;
    for (size_t i=0;i<readn;i++) {
        unsigned char ch = buf[i];
        char *code = codes[ch];
        // 对每个 '0'/'1' 写入对应的位
        for (size_t k=0; code && code[k]; k++) {
            if (code[k] == '1') outbyte |= (1u << (7-outbit));
            outbit++;
            if (outbit == 8) {
                fwrite(&outbyte, 1, 1, out);
                outbyte = 0; outbit = 0;
            }
        }
    }
    // 写入可能剩余的半字节（用 0 补齐高位）
    if (outbit > 0) fwrite(&outbyte, 1, 1, out);

    // Cleanup
    fclose(out);
    free(buf);
    for (int i=0;i<256;i++) if (codes[i]) free(codes[i]);
    node_free(root);
    return 0;
}

// 解压主流程
static int decompress_file(const char *inpath, const char *outpath) {
    FILE *in = fopen(inpath, "rb");
    if (!in) { perror("fopen input"); return 1; }
    // 读 header
    uint32_t orig_size;
    if (fread(&orig_size, sizeof(orig_size), 1, in) != 1) { fclose(in); fprintf(stderr, "invalid input\n"); return 1; }
    uint32_t freq[256];
    uint64_t total = 0;
    for (int i=0;i<256;i++) {
        if (fread(&freq[i], sizeof(freq[i]), 1, in) != 1) { fclose(in); fprintf(stderr, "invalid header\n"); return 1; }
        total += freq[i];
    }
    if (total != orig_size) {
        // 可能文件较大，orig_size 为 uint32_t；但通常一致。这里不强制退出，只警告。
        // fprintf(stderr, "warning: header size mismatch\n");
    }

    // 重新构建哈夫曼树（同压缩时的逻辑）
    Node *nodes[512]; int n_nodes=0;
    for (int c=0;c<256;c++) if (freq[c] > 0) nodes[n_nodes++] = node_new((unsigned char)c, freq[c], NULL, NULL);
    if (n_nodes == 0) {
        // 空文件：只创建输出文件
        FILE *out = fopen(outpath, "wb"); if (!out) { perror("fopen out"); fclose(in); return 1; } fclose(out); fclose(in); return 0;
    }
    if (n_nodes == 1) {
        Node *only = nodes[0];
        Node *root = node_new(0, only->freq, only, NULL);
        nodes[0] = root; n_nodes = 1;
    } else {
        while (n_nodes > 1) {
            int i1=-1,i2=-1; pick_two_min(nodes, n_nodes, &i1, &i2);
            Node *a = nodes[i1]; Node *b = nodes[i2];
            Node *p = node_new(0, a->freq + b->freq, a, b);
            nodes[i1] = p; nodes[i2] = nodes[n_nodes-1]; n_nodes--;
        }
    }
    Node *root = nodes[0];

    FILE *out = fopen(outpath, "wb");
    if (!out) { perror("fopen out"); node_free(root); fclose(in); return 1; }

    // 读取剩余字节并逐位解码，直到输出 orig_size 字节
    unsigned char inbyte;
    int inbit = 8; // 促使第一次读取字节
    Node *cur = root;
    uint32_t written = 0;
    while (written < orig_size && fread(&inbyte, 1, 1, in) == 1) {
        for (int b=0;b<8 && written < orig_size; b++) {
            int bit = (inbyte >> (7-b)) & 1;
            cur = bit ? cur->right : cur->left;
            if (!cur->left && !cur->right) {
                // 叶子，输出字符
                fputc(cur->ch, out);
                written++;
                cur = root;
                if (written >= orig_size) break;
            }
        }
    }

    fclose(out);
    node_free(root);
    fclose(in);
    return 0;
}

static void print_usage(const char *p) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s -c input.txt output.huf   # 压缩\n", p);
    fprintf(stderr, "  %s -d input.huf output.txt   # 解压\n", p);
}

int main(int argc, char **argv) {
    if (argc != 4) { print_usage(argv[0]); return 1; }
    if (strcmp(argv[1], "-c") == 0) {
        return compress_file(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-d") == 0) {
        return decompress_file(argv[2], argv[3]);
    } else {
        print_usage(argv[0]);
        return 1;
    }
}
