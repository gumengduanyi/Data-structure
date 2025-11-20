#include <stdio.h>

// 判断是否为回文数
int loop(int x) {
    int original = x;
    int reversed = 0;
    while (x > 0) {
        reversed = reversed * 10 + x % 10;
        x /= 10;
    }
    return reversed == original;
}

int main() {
    int a, b;
    int i;

    scanf("%d %d", &a, &b);

    for (i = a; i <= b; i++) {
        if (loop(i)) {
            printf("%d\n", i);
        }
    }

    return 0;
}
