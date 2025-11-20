#include <stdio.h>


int leap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int days(int y, int m, int d) {
    int i;
    int day = d;
    int mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (leap(y)) {
        mdays[1] = 29;
    }

    for (i = 0; i < m - 1; i++) {
        day += mdays[i];
    }

    return day;
}

int main() {
    int y, a, b, c, d;
    int start_day, target_day, diff;

    
    scanf("%d-%d-%d", &y, &a, &b);
    
    scanf("%d-%d", &c, &d);

    start_day = days(y, a, b);
    target_day = days(y, c, d);
    diff = target_day - start_day;

    if (diff % 5 < 3) {
        printf("1\n");  
    } else {
        printf("0\n"); 
    }

    return 0;
}
