/* Runs the same C source natively on this machine, for comparison. */
#include <stdio.h>
#include "sum_clamped.c"

int main(void)
{
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printf("native: sum_clamped(1..10, lo=0, hi=100) = %d\n", sum_clamped(a, 10, 0, 100));
    return 0;
}
