#include <stdio.h>

int search(int list[], int n, int x) {
    int i;
    for (i = 0; i < n; i++) {
        if (list[i] == x) {
            return i;
        }
    }
    return -1;
}

int main() {
    int n, x;
    int list[10];
    int i;

    printf("Input n:");
    scanf("%d", &n);

    printf("Input %d integers:", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &list[i]);
    }

    printf("Input x:");
    scanf("%d", &x);

    int index = search(list, n, x);
    if (index != -1) {
        printf("index = %d\n", index);
    } else {
        printf("Not found\n");
    }

    return 0;
}
