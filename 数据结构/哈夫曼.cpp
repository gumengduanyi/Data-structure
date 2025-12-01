// 哈夫曼编码/解码示例（C++17）
// 使用示例：
//   编码: ./huffman -e input.txt output.huff
//   解码: ./huffman -d input.huff output.txt
// 文件格式（头部）简述：
// 4 字节魔数 "HUF\1"，uint32_t 唯一符号数，随后每个条目为 (uint8_t symbol, uint64_t freq)，
// 接着 uint64_t 原始字节数（original_size），uint64_t 编码位数（bit_count），紧接编码数据（按字节打包）

#include <bits/stdc++.h>
using namespace std;

struct Node {
    uint64_t freq;
    unsigned char ch; // 只有叶子有效
    Node *l, *r;
    bool leaf;
    Node(uint64_t f=0, unsigned char c=0, bool isleaf=false): freq(f), ch(c), l(nullptr), r(nullptr), leaf(isleaf) {}
};

struct Cmp {
    bool operator()(const Node* a, const Node* b) const {
        return a->freq > b->freq; // min-heap
    }
};

// 释放树
void free_tree(Node* root) {
    if (!root) return;
    free_tree(root->l);
    free_tree(root->r);
    delete root;
}

// 生成哈夫曼编码表（字节 -> bitstring）
void build_codes(Node* root, vector<string> &codes, string &path) {
    if (!root) return;
    if (root->leaf) {
        // 叶子：记录 code
        codes[root->ch] = path.empty() ? string("0") : path; // 单字符情况给个 '0'
        return;
    }
    path.push_back('0');
    build_codes(root->l, codes, path);
    path.back() = '1';
    build_codes(root->r, codes, path);
    path.pop_back();
}

// 写原始类型到流（按本机字节序写二进制）
template<typename T>
void write_binary(ostream &os, const T &v) {
    os.write(reinterpret_cast<const char*>(&v), sizeof(T));
}

template<typename T>
bool read_binary(istream &is, T &v) {
    is.read(reinterpret_cast<char*>(&v), sizeof(T));
    return bool(is);
}

// 编码函数
bool encode_file(const string &infile, const string &outfile) {
    // 读取整个输入文件为字节向量
    ifstream ifs(infile, ios::binary);
    if (!ifs) { cerr << "无法打开输入文件: " << infile << '\n'; return false; }
    vector<unsigned char> data((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    uint64_t orig_size = data.size();

    // 统计频率
    array<uint64_t, 256> freq{};
    for (unsigned char c : data) freq[c]++;

    // 构造优先队列
    priority_queue<Node*, vector<Node*>, Cmp> pq;
    for (int i = 0; i < 256; ++i) if (freq[i] > 0) {
        Node *n = new Node(freq[i], (unsigned char)i, true);
        pq.push(n);
    }

    if (pq.empty()) {
        // 空文件，写空头并返回
        ofstream ofs(outfile, ios::binary);
        if (!ofs) return false;
        ofs.write("HUF\1", 4);
        uint32_t zero = 0; write_binary(ofs, zero);
        uint64_t z64 = 0; write_binary(ofs, z64); write_binary(ofs, z64);
        return true;
    }

    // 特殊情况：只有一个符号
    if (pq.size() == 1) {
        Node *only = pq.top(); pq.pop();
        Node *root = new Node(only->freq, 0, false);
        root->l = only; root->r = nullptr; // make a parent so decoder can traverse
        // 产生编码表
        vector<string> codes(256);
        string path;
        build_codes(root, codes, path);

        // 写文件头
        ofstream ofs(outfile, ios::binary);
        if (!ofs) return false;
        ofs.write("HUF\1", 4);
        // 唯一符号数
        uint32_t uniq = 1; write_binary(ofs, uniq);
        // 写 (symbol, freq)
        unsigned char sym = only->ch; ofs.write(reinterpret_cast<const char*>(&sym), 1);
        write_binary(ofs, only->freq);
        // 原始长度与位数
        uint64_t bit_count = 0;
        for (unsigned char c : data) bit_count += codes[c].size();
        write_binary(ofs, orig_size);
        write_binary(ofs, bit_count);
        // 写编码数据（按位打包）
        unsigned char curbyte = 0; int bitpos = 0;
        for (unsigned char c : data) {
            const string &s = codes[c];
            for (char b : s) {
                if (b == '1') curbyte |= (1u << (7 - bitpos));
                bitpos++;
                if (bitpos == 8) { ofs.put((char)curbyte); curbyte = 0; bitpos = 0; }
            }
        }
        if (bitpos) ofs.put((char)curbyte);
        free_tree(root);
        return true;
    }

    // 常规构造哈夫曼树
    while (pq.size() > 1) {
        Node *a = pq.top(); pq.pop();
        Node *b = pq.top(); pq.pop();
        Node *p = new Node(a->freq + b->freq, 0, false);
        p->l = a; p->r = b;
        pq.push(p);
    }
    Node *root = pq.top();

    // 生成编码表
    vector<string> codes(256);
    string path;
    build_codes(root, codes, path);

    // 计算位数
    uint64_t bit_count = 0;
    for (unsigned char c : data) bit_count += codes[c].size();

    // 写输出文件（头 + 编码数据）
    ofstream ofs(outfile, ios::binary);
    if (!ofs) { free_tree(root); return false; }
    ofs.write("HUF\1", 4);
    // 写唯一符号数
    uint32_t uniq = 0;
    for (int i = 0; i < 256; ++i) if (freq[i] > 0) ++uniq;
    write_binary(ofs, uniq);
    // 写频率表（symbol + freq）
    for (int i = 0; i < 256; ++i) if (freq[i] > 0) {
        unsigned char sym = (unsigned char)i; ofs.write(reinterpret_cast<const char*>(&sym), 1);
        write_binary(ofs, freq[i]);
    }
    // 写原始大小与位数
    write_binary(ofs, orig_size);
    write_binary(ofs, bit_count);

    // 写位流（打包为字节，MSB-first）
    unsigned char curbyte = 0; int bitpos = 0; // bitpos: 已填入当前字节的位数（0..7）
    for (unsigned char c : data) {
        const string &s = codes[c];
        for (char b : s) {
            if (b == '1') curbyte |= (1u << (7 - bitpos));
            bitpos++;
            if (bitpos == 8) { ofs.put((char)curbyte); curbyte = 0; bitpos = 0; }
        }
    }
    if (bitpos) ofs.put((char)curbyte);

    free_tree(root);
    return true;
}

// 解码函数
bool decode_file(const string &infile, const string &outfile) {
    ifstream ifs(infile, ios::binary);
    if (!ifs) { cerr << "无法打开压缩文件: " << infile << '\n'; return false; }
    char magic[4]; ifs.read(magic, 4);
    if (strncmp(magic, "HUF\1", 4) != 0) { cerr << "不是有效的 HUF 文件\n"; return false; }
    uint32_t uniq = 0; read_binary(ifs, uniq);
    array<uint64_t,256> freq{};
    for (uint32_t i = 0; i < uniq; ++i) {
        unsigned char sym; ifs.read(reinterpret_cast<char*>(&sym), 1);
        uint64_t f; read_binary(ifs, f);
        freq[sym] = f;
    }
    uint64_t orig_size = 0; read_binary(ifs, orig_size);
    uint64_t bit_count = 0; read_binary(ifs, bit_count);

    // 构造树（与编码端相同逻辑）
    priority_queue<Node*, vector<Node*>, Cmp> pq;
    for (int i = 0; i < 256; ++i) if (freq[i] > 0) {
        pq.push(new Node(freq[i], (unsigned char)i, true));
    }
    if (pq.empty()) {
        // 空输入，写空文件
        ofstream ofs(outfile, ios::binary);
        return bool(ofs);
    }
    if (pq.size() == 1) {
        Node *only = pq.top(); pq.pop();
        Node *root = new Node(only->freq, 0, false); root->l = only; root->r = nullptr;
        // 读取位流并解码
        vector<char> out; out.reserve(orig_size);
        uint64_t bits_read = 0;
        unsigned char curbyte;
        while (bits_read < bit_count && ifs.get((char&)curbyte)) {
            for (int i = 0; i < 8 && bits_read < bit_count; ++i, ++bits_read) {
                int bit = (curbyte >> (7 - i)) & 1;
                // 在只有一个符号的情况下，无论 0/1 都代表该符号
                out.push_back((char)only->ch);
                if (out.size() >= orig_size) break;
            }
        }
        ofstream ofs(outfile, ios::binary);
        ofs.write(out.data(), out.size());
        free_tree(root);
        return true;
    }
    while (pq.size() > 1) {
        Node *a = pq.top(); pq.pop();
        Node *b = pq.top(); pq.pop();
        Node *p = new Node(a->freq + b->freq, 0, false);
        p->l = a; p->r = b; pq.push(p);
    }
    Node *root = pq.top();

    // 读取编码数据按位解码
    ofstream ofs(outfile, ios::binary);
    if (!ofs) { free_tree(root); return false; }
    vector<char> out; out.reserve(orig_size);
    uint64_t bits_read = 0;
    unsigned char curbyte;
    Node *node = root;
    while (bits_read < bit_count && ifs.get((char&)curbyte)) {
        for (int i = 0; i < 8 && bits_read < bit_count; ++i, ++bits_read) {
            int bit = (curbyte >> (7 - i)) & 1;
            node = bit ? node->r : node->l;
            if (!node) { free_tree(root); cerr << "解码错误：遇到空节点\n"; return false; }
            if (node->leaf) {
                out.push_back((char)node->ch);
                node = root;
                if (out.size() >= orig_size) break;
            }
        }
    }
    // 写出原始字节
    if (!out.empty()) ofs.write(out.data(), out.size());
    free_tree(root);
    return true;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        cerr << "用法: " << argv[0] << " -e|-d input output\n";
        return 1;
    }
    string mode = argv[1];
    string in = argv[2];
    string out = argv[3];
    bool ok = false;
    if (mode == string("-e")) ok = encode_file(in, out);
    else if (mode == string("-d")) ok = decode_file(in, out);
    else { cerr << "未知模式: " << mode << '\n'; return 1; }
    if (!ok) { cerr << "操作失败\n"; return 2; }
    return 0;
}
