/*
 * 哈夫曼编码/解码（C 语言实现，兼容 C/C++ 编译）
 * 用法：程序支持 -c|-d|-s 三个选项（见 main()）。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//在压缩文件的开头写入这 4 字节，解压时读取并用它来快速验证文件是否为本程序生成的 Huffman 文件（文件类型/版本检测）。
#define MAGIC "HUF1"

// 哈夫曼树节点
typedef struct HNode {
	unsigned char ch;
	uint64_t freq;
	struct HNode *l, *r; // 左右子节点指针
} HNode;


// 最小堆（简单数组实现，用于构建 Huffman）
/*
 * 说明：
 *  本代码使用一个基于数组的二叉最小堆来管理当前的节点集合，原因有三个：
 *  1) Huffman 建树需要反复执行“取最小的两个节点”并把合并后的父节点重新插入集合，
 *     因此需要高效的「取最小」和「插入」操作。二叉最小堆能以 O(log m) 的代价完成这两项操作，
 *     整体建树时间为 O(m log m)，m 为不同符号数。
 *  2) 数组实现简单、轻量，且堆中保存的是 `HNode *`（指针），交换时仅交换指针，开销很低。
 *  3) 在纯 C 环境下实现方便；若为 C++，可直接用 `std::priority_queue`。
 *
 * 实现注意点：
 *  - `data` 存放 `HNode*` 指针数组，`size` 为当前元素数，`cap` 为容量。代码使用 `realloc`
 *    动态扩容（成倍增长），以减少重分配次数。
 *  - `heap_destroy` 只释放数组本身，不释放堆中指向的 `HNode`，因为节点在合并后由哈夫曼树统一释放。
 *  - 当多个节点频率相同时，合并顺序会影响最终编码（但不会影响解压正确性）。若需要确定性输出，
 *    可以在节点中加入一个插入序号作为 tiebreaker（即使频率相同也按先后顺序合并）。
 *
 * 复杂度：
 *  - 时间：O(m log m)，m 为不同符号数。空间：O(m) 额外指针数组。
 *
 * 替代数据结构比较：
 *  - 有序链表/数组：插入需 O(m)，总成本高（O(m^2)），不推荐。
 *  - 平衡树（红黑树等）：也能达到 O(log m)，但实现更复杂。
 *  - 高级堆（配对堆、斐波那契堆）：插入/合并在摊销上可能更优，但实现复杂且对本问题收益有限。
 */
typedef struct MinHeap
{
	HNode **data; // 指向 HNode* 的数组
	size_t size;  // 当前元素数
	size_t cap;   // 容量
} MinHeap;

/* 前向声明：node_create 在文件后部定义，但在 build_huffman 等函数中被调用，
 * 因此在此声明其原型以便编译器知晓。 */
static HNode* node_create(unsigned char ch, uint64_t freq);

// (node_create 已移动至堆函数组之后，便于按功能组织)

// 创建堆
/* 创建最小堆（用于构建 Huffman 树）
 * cap: 初始容量（元素个数）
 * 返回：分配好的堆指针，失败返回 NULL
 */
static MinHeap* heap_create(size_t cap)
{
	MinHeap *h = (MinHeap*)malloc(sizeof(MinHeap));
	if (!h)
	{
		return NULL;
	}
	/* data 存放 HNode* 指针数组，动态分配 cap 个元素空间 */
	h->data = (HNode**)malloc(sizeof *h->data * cap);
	if (!h->data)
	{
		free(h);
		return NULL;
	}
	h->size = 0;
	h->cap = cap;
	return h;
}

// 释放堆分配的内存（不释放堆中 HNode 指向的节点）
static void heap_destroy(MinHeap *h)
{
	if (!h)
	{
		return;
	}
	free(h->data);
	free(h);
}

// 交换堆数组中的两个元素（指针交换）
static void heap_swap(HNode **a, HNode **b)
{
	HNode *t = *a;
	*a = *b;
	*b = t;
}

// 向堆中压入节点
/* 将节点推入最小堆（基于数组的二叉堆）
 * 过程：
 *  1. 若容量不足，尝试扩容为原来的两倍
 *  2. 将节点放在数组末尾（位置 i），然后执行上浮操作
 */
static void heap_push(MinHeap *h, HNode *node)
{
	if (h->size >= h->cap)
	{
		size_t newcap = h->cap * 2;
		HNode **tmp = (HNode**)realloc(h->data, sizeof *h->data * newcap);
		if (!tmp)
		{
			return; /* realloc 失败时保持原堆不变（调用者应检测内存） */
		}
		h->data = tmp;
		h->cap = newcap;
	}
	size_t i = h->size++;
	h->data[i] = node; /* 放到末尾 */
	/* 上浮：如果父节点频率更大则交换，直到堆序恢复 */
	while (i > 0)
	{
		size_t p = (i - 1) / 2;
		if (h->data[p]->freq <= h->data[i]->freq)
		{
			break;
		}
		heap_swap(&h->data[p], &h->data[i]);
		i = p;
	}
}


/* 从最小堆弹出最小频率的节点（堆顶）
 * 返回被弹出的节点指针，堆内不再持有该指针
 */
static HNode* heap_pop(MinHeap *h)
{
	if (h->size == 0)
	{
		return NULL;
	}
	HNode *ret = h->data[0]; /* 保存堆顶 */
	h->size--; /* 减少元素数 */
	h->data[0] = h->data[h->size]; /* 将末尾元素移到堆顶 */
	size_t i = 0;
	/* 下沉操作：与子节点比较并交换直到堆序恢复 */
	while (1)
	{
		size_t l = i * 2 + 1;
		size_t r = i * 2 + 2;
		size_t smallest = i;
		if (l < h->size && h->data[l]->freq < h->data[smallest]->freq)
		{
			smallest = l;
		}
		if (r < h->size && h->data[r]->freq < h->data[smallest]->freq)
		{
			smallest = r;
		}
		if (smallest == i)
		{
			break;
		}
		heap_swap(&h->data[i], &h->data[smallest]);
		i = smallest;
	}
	return ret;
}

// 构建哈夫曼树
/* 根据频率表构建哈夫曼树
 * freq: 长度 256 的频率数组（每个字节对应的出现次数）
 * 返回：哈夫曼树根节点（调用者负责释放树）
 */
static HNode* build_huffman(uint64_t freq[256])
{
	/* 统计出现次数非零的符号数量 */
	size_t nonzero = 0;
	for (int i = 0; i < 256; ++i)
	{
		if (freq[i])
		{
			++nonzero;
		}
	}
	if (nonzero == 0)
	{
		return NULL; /* 空输入，没有符号 */
	}

	/* 创建一个容量合适的最小堆 */
	MinHeap *h = heap_create(nonzero > 8 ? nonzero * 2 : 16);
	if (!h)
	{
		return NULL;
	}

	/* 将所有非零频率的符号作为叶子节点插入堆中 */
	for (int i = 0; i < 256; ++i)
	{
		if (freq[i])
		{
			HNode *n = node_create((unsigned char)i, freq[i]);
			heap_push(h, n);
		}
	}

	/* 如果只有一种符号，直接返回该单节点树（无需合并） */
	if (h->size == 1)
	{
		HNode *root = heap_pop(h);
		heap_destroy(h);
		return root;
	}

	/* 标准 Huffman 合并过程：每次弹出两个最小频率节点，合并为父节点并入堆 */
	while (h->size > 1)
	{
		HNode *a = heap_pop(h);
		HNode *b = heap_pop(h);
		/* node_create 在此处可用以创建父节点（移动后的定义位于下方） */
		HNode *p = node_create(0, a->freq + b->freq);
		p->l = a;
		p->r = b;
		heap_push(h, p);
	}
	HNode *root = heap_pop(h);
	heap_destroy(h);
	return root;
}

// 生成编码表：codes[i] 保存为 "+" 结尾的字符串 '0'/'1'（动态分配）
// codes 数组保存每个字节对应的编码字符串（'0'/'1'，以 '\0' 结尾）
static char *codes[256];

// 创建一个新的哈夫曼树节点并初始化字段
// 参数：
//   ch   - 节点代表的字节（仅对叶子有意义）
//   freq - 该字节的频率（出现次数）
// 返回：分配好的节点指针，失败时返回 NULL
static HNode* node_create(unsigned char ch, uint64_t freq)
{
	HNode *n = (HNode*)malloc(sizeof(HNode)); // 分配内存
	if (!n)
	{
		return NULL; // 分配失败，返回 NULL
	}
	n->ch = ch;    // 存储字符
	n->freq = freq; // 存储频率
	n->l = n->r = NULL; // 初始化左右子指针为 NULL
	return n; // 返回新节点
}

/* 递归生成编码表
 * node: 当前遍历到的哈夫曼节点
 * buf: 临时缓冲用于保存路径（'0' 表示向左，'1' 表示向右）
 * depth: 当前深度（buf 的下标）
 */
static void gen_codes(HNode *node, char *buf, int depth)
{
	if (!node)
	{
		return;
	}
	if (!node->l && !node->r)
	{
		/* 到达叶子：将缓冲中的路径复制为该字节的编码 */
		buf[depth] = '\0';
		codes[node->ch] = (char*)malloc(depth + 1);
		if (codes[node->ch])
		{
			memcpy(codes[node->ch], buf, depth + 1);
		}
		return;
	}
	if (node->l)
	{
		buf[depth] = '0';
		gen_codes(node->l, buf, depth + 1);
	}
	if (node->r)
	{
		buf[depth] = '1';
		gen_codes(node->r, buf, depth + 1);
	}
}

// 递归释放哈夫曼树的所有节点
static void free_tree(HNode *node)
{
	if (!node)
	{
		return;
	}
	free_tree(node->l);
	free_tree(node->r);
	free(node);
}

// Helpers: 固定为 little-endian 的 64-bit 读写（保证跨平台字节序一致）
static int write_u64_le(FILE *f, uint64_t v)
{
	unsigned char b[8];
	for (int i = 0; i < 8; ++i)
	{
		b[i] = (unsigned char)((v >> (8 * i)) & 0xFF);
	}
	return fwrite(b, 1, 8, f) == 8 ? 0 : -1;
}

static int read_u64_le(FILE *f, uint64_t *out)
{
	unsigned char b[8];
	if (fread(b, 1, 8, f) != 8)
	{
		return -1;
	}
	uint64_t v = 0;
	for (int i = 0; i < 8; ++i)
	{
		v |= (uint64_t)b[i] << (8 * i);
	}
	*out = v;
	return 0;
}

/* 位写入器：按位缓冲写入到文件
 * buf 保存当前未写出的字节，pos 表示已填充的位数（0..7）
 * bw_write 每调用一次写入一个 bit（低位优先），当 pos==8 时写入文件
 * bw_flush 将残余位（若有）输出为一个字节
 */
typedef struct BitWriter {
	/* 输出文件句柄（非 NULL）：
	 * 说明（标准头部格式）：
	 *  - 偏移 0..3   : 4 字节魔数 (ASCII) = "HUF1" （用于文件类型/版本检测）
	 *  - 偏移 4..11  : 8 字节无符号整数，原始未压缩字节数 (uint64_t, little-endian)
	 *  - 偏移 12..(12+2048-1) : 256 个 uint64_t 的频率表（每个符号对应一个 8 字节的出现次数，均为 little-endian）
	 *  合计头部长度 = 4 + 8 + 256*8 = 2060 字节。位流紧随其后，直接从文件的第 2060 字节开始。
	 *
	 * 行为与约定（常见用法）：
	 *  写入端（压缩）常见流程：
	 *    1. `FILE *f = fopen(outpath, "wb");` 打开输出文件（检查返回值）。
	 *    2. 写入魔数：`fwrite(MAGIC, 1, 4, f);`。
	 *    3. 写入原始长度：`write_u64_le(f, orig_size);`。
	 *    4. 写入 256 个频率值（按索引 0..255）：`for(i=0;i<256;i++) write_u64_le(f, freq[i]);`。
	 *    5. 初始化位写入器：`BitWriter bw; bw_init(&bw, f);`。
	 *    6. 逐位调用 `bw_write(&bw, bit);`（本实现采用 LSB-first，详见下文）。
	 *    7. 完成后调用 `bw_flush(&bw);` 以写出残余位，然后 `fclose(f);`。
	 *
	 *  解码端（解压）常见流程：
	 *    1. `FILE *f = fopen(inpath, "rb");` 打开输入文件（检查返回值）。
	 *    2. 读取并验证魔数/原始长度/频率表：`fread(magic,1,4,f); read_u64_le(f,&orig_size); for(i=0;i<256;i++) read_u64_le(f,&freq[i]);`。
	 *    3. 基于频率表重建 Huffman 树。
	 *    4. 初始化位读取器：`BitReader br; br_init(&br, f);`。
	 *    5. 通过 `br_read(&br)` 逐位读取（返回 0/1，遇 EOF 返回 -1），沿树遍历直到输出 `orig_size` 个字节为止。
	 *
	 * 位顺序与语义：
	 *  - 本实现使用 LSB-first（低位优先）顺序：即在每次写入时，首个写入的 bit 对应字节的 bit0（1u<<0），
	 *    第二个写入的 bit 对应 bit1，以此类推，直至 bit7，然后输出该字节再从 bit0 开始新字节。
	 *  - `bw_write` 将位累积在 `buf` 中并在填满 8 位时通过 `fwrite` 写出；`bw_flush` 会在结尾写出不满 8 位的残余字节，
	 *    残余字节的高位未被写入的位一律为 0。解码端必须依据 `orig_size` 停止输出，忽略压缩流末尾填充位。
	 *
	 * 错误处理与注意事项：
	 *  - `BitWriter` 本身不打开或关闭 `FILE *f`，也不在内部记录 `fwrite` 的错误码；调用方应检查 I/O 返回值并负责关闭。
	 *  - 不要在写入 header 之后向同一文件插入其他数据，否则会破坏解码端对头部和位流的解析。
	 */
	FILE *f; /* 输出目标文件句柄（由调用方打开/关闭）；位写入器在缓冲满或 flush 时向此句柄写出字节 */
	unsigned char buf;
	int pos; // 已填充位数（0-7）
} BitWriter;

static void bw_init(BitWriter *bw, FILE *f)
{
	bw->f = f;
	bw->buf = 0;
	bw->pos = 0;
}

static void bw_write(BitWriter *bw, int bit)
{
	if (bit)
	{
		bw->buf |= (1u << bw->pos); /* 将 bit 写入当前缓冲的 pos 位（低位优先） */
	}
	bw->pos++;
	if (bw->pos == 8)
	{
		fwrite(&bw->buf, 1, 1, bw->f); /* 写满一个字节则输出 */
		bw->buf = 0;
		bw->pos = 0;
	}
}

static void bw_flush(BitWriter *bw)
{
	if (bw->pos > 0)
	{
		fwrite(&bw->buf, 1, 1, bw->f); /* 输出残余位（高位为 0） */
		bw->buf = 0;
		bw->pos = 0;
	}
}

/* 位读取器：从文件按位读取
 * pos==8 表示需要读取新字节到 buf；返回 0/1 表示读到的位，遇到文件末尾返回 -1
 */
typedef struct BitReader {
	FILE *f;
	unsigned char buf;
	int pos; // 下一个要读的位置（0-7），如果 pos==8 表示需读新字节
	int eof;
} BitReader;

static void br_init(BitReader *br, FILE *f)
{
	br->f = f;
	br->buf = 0;
	br->pos = 8;
	br->eof = 0;
}

static int br_read(BitReader *br)
{
	if (br->pos == 8)
	{
		size_t n = fread(&br->buf, 1, 1, br->f);
		if (n != 1)
		{
			br->eof = 1;
			return -1;
		}
		br->pos = 0;
	}
	int bit = (br->buf >> br->pos) & 1u; /* 取出当前 pos 位（低位优先） */
	br->pos++;
	return bit;
}

// 压缩流程
static int compress_file(const char *inpath, const char *outpath)
{
	// 第一遍：按块统计频率
	FILE *fin = fopen(inpath, "rb");
	if (!fin)
	{
		perror("fopen input");
		return 1;
	}
	uint64_t freq[256]; 
	memset(freq, 0, sizeof freq);
	unsigned char *buf = (unsigned char*)malloc(BUFSIZ);
	if (!buf)
	{
		perror("malloc");
		fclose(fin);
		return 1;
	}
	size_t n;
	uint64_t total = 0;
	while ((n = fread(buf, 1, BUFSIZ, fin)) > 0) {
		for (size_t i = 0; i < n; ++i) 
		freq[buf[i]]++;
		total += n;
	}
	fclose(fin);

	// 构建哈夫曼树
	HNode *root = build_huffman(freq);

	// 如果原始文件为空，写空头并退出
	if (!root && total == 0)
	{
		FILE *fout = fopen(outpath, "wb");
		if (!fout)
		{
			perror("fopen output");
			free(buf);
			return 1;
		}
		fwrite(MAGIC, 1, 4, fout);
		uint64_t zero = 0;
		/* 使用 little-endian 固定字节序写入 orig_size 和频率表 */
		if (write_u64_le(fout, zero) != 0)
		{
			perror("write orig size");
			fclose(fout);
			free(buf);
			return 1;
		}
		for (int i = 0; i < 256; ++i)
		{
			if (write_u64_le(fout, zero) != 0)
			{
				perror("write freq");
				fclose(fout);
				free(buf);
				return 1;
			}
		}
		fclose(fout);
		free(buf);
		return 0;
	}

	// 生成编码表
	for (int i = 0; i < 256; ++i)
	{
		if (codes[i])
		{
			free(codes[i]);
			codes[i] = NULL;
		}
	}
	char tmpbuf[512];
	gen_codes(root, tmpbuf, 0);

	// 打开输出并先写头（魔数 + 原始长度 + 频率表）
	FILE *fout = fopen(outpath, "wb");
	if (!fout)
	{
		perror("fopen output");
		free_tree(root);
		free(buf);
		return 1;
	}
	fwrite(MAGIC, 1, 4, fout);
	uint64_t orig_size = total;
	// 写入头部：magic + orig_size + 256 个 uint64（都用 little-endian）
	if (write_u64_le(fout, orig_size) != 0)
	{
		perror("write orig size");
		fclose(fout);
		free_tree(root);
		free(buf);
		return 1;
	}
	for (int i = 0; i < 256; ++i)
	{
		uint64_t v = freq[i];
		if (write_u64_le(fout, v) != 0)
		{
			perror("write freq");
			fclose(fout);
			free_tree(root);
			free(buf);
			return 1;
		}
	}

	// 第二遍：逐块读取并编码写入
	BitWriter bw;
	bw_init(&bw, fout);
	fin = fopen(inpath, "rb");
	if (!fin)
	{
		perror("fopen input second pass");
		fclose(fout);
		free_tree(root);
		free(buf);
		return 1;
	}
	// 特殊情况：哈夫曼树只有一个符号，codes[...] 可能为空串，处理为写单个位 0
	int single_symbol = (root && root->l==NULL && root->r==NULL);

	while ((n = fread(buf, 1, BUFSIZ, fin)) > 0)
	{
		for (size_t i = 0; i < n; ++i)
		{
			unsigned char ch = buf[i];
			char *code = codes[ch];
			if (single_symbol)
			{
				/* 若只有一个符号，确保至少写一位（写 0） */
				bw_write(&bw, 0);
			}
			else
			{
				if (!code)
				{
					continue;
				}
				for (size_t k = 0; code[k]; ++k)
				{
					bw_write(&bw, code[k] == '1');
				}
			}
		}
	}
	bw_flush(&bw);
	fclose(fin);
	fclose(fout);

	/* 释放 */
	free_tree(root);
	for (int i = 0; i < 256; ++i)
	{
		if (codes[i])
		{
			free(codes[i]);
			codes[i] = NULL;
		}
	}
	free(buf);
	return 0;
}

// 解压流程
static int decompress_file(const char *inpath, const char *outpath)
{
	FILE *fin = fopen(inpath, "rb");
	if (!fin)
	{
		perror("fopen input");
		return 1;
	}
	char magic[4];
	if (fread(magic, 1, 4, fin) != 4)
	{
		fprintf(stderr, "input too small\n");
		fclose(fin);
		return 1;
	}
	if (memcmp(magic, MAGIC, 4) != 0)
	{
		fprintf(stderr, "bad magic\n");
		fclose(fin);
		return 1;
	}
	uint64_t orig_size;
	if (read_u64_le(fin, &orig_size) != 0)
	{
		fprintf(stderr, "read orig size fail\n");
		fclose(fin);
		return 1;
	}
	uint64_t freq[256];
	for (int i = 0; i < 256; ++i)
	{
		if (read_u64_le(fin, &freq[i]) != 0)
		{
			fprintf(stderr, "read freq fail\n");
			fclose(fin);
			return 1;
		}
	}

	HNode *root = build_huffman(freq);
	if (!root)
	{
		/* 原始为空文件，创建空输出 */
		FILE *fout = fopen(outpath, "wb");
		if (fout)
		{
			fclose(fout);
		}
		fclose(fin);
		return 0;
	}

	FILE *fout = fopen(outpath, "wb");
	if (!fout)
	{
		perror("fopen output");
		free_tree(root);
		fclose(fin);
		return 1;
	}

	// 特殊情况：树只有一个符号
	if (root->l == NULL && root->r == NULL)
	{
		/* 直接写 orig_size 个 root->ch */
		for (uint64_t i = 0; i < orig_size; ++i)
		{
			fputc(root->ch, fout);
		}
		fclose(fout);
		free_tree(root);
		fclose(fin);
		return 0;
	}

	BitReader br; br_init(&br, fin);
	HNode *cur = root;
	uint64_t written = 0;
	while (written < orig_size) {
		int bit = br_read(&br);
		if (bit < 0) break; // 提前 EOF
		if (bit) cur = cur->r; else cur = cur->l;
		if (!cur) break; // 错误
		if (!cur->l && !cur->r) {
			fputc(cur->ch, fout);
			written++;
			cur = root;
		}
	}

	fclose(fout);
	free_tree(root);
	fclose(fin);
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s -c|-d input output\n", argc>0?argv[0]:"huffman");
		return 1;
	}
	if (strcmp(argv[1], "-c") == 0)
	{
		return compress_file(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "-d") == 0)
	{
		return decompress_file(argv[2], argv[3]);
	}
	else
	{
		fprintf(stderr, "Unknown option %s\n", argv[1]);
		return 1;
	}
}

