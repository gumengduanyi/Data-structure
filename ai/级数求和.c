#include <stdio.h>

int main() {
    int n, i;
    printf("Input n: ");
    scanf("%d", &n);

    double sum = 1.0;
    double term = 1.0;

    for (i = 1; i <= n; i++) {
        term = term / i;
        sum += term;
    }

    printf("e=%.4f\n", sum);
    return 0;
}
