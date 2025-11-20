#include <stdio.h>

void sort(int a[], int n) {
    int i, j, min_idx, temp;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (a[j] < a[min_idx]) {
                min_idx = j;
            }
        }
        temp = a[i];
        a[i] = a[min_idx];
        a[min_idx] = temp;
    }
}

int main() {
    int n, i;
    int a[100];

    printf("Input n:");
    scanf("%d", &n);

    printf("Input array of %d integers:", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    sort(a, n);

    printf("After sorted the array is:");
    for (i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
    printf("\n");

    return 0;
}
