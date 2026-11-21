/* Runs the live-typed sum_to natively on this machine, for comparison. */
#include <stdio.h>
#include <stdlib.h>

int sum_to(int n);

int main(int argc, char **argv)
{
    int n = argc > 1 ? atoi(argv[1]) : 10;
    printf("native: sum_to(%d) = %d\n", n, sum_to(n));
    return 0;
}
