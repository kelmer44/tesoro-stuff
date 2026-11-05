/* Demo function: sum an array, clamping each element to [lo, hi]. */
int sum_clamped(const int *a, int n, int lo, int hi)
{
    int total = 0;
    for (int i = 0; i < n; i++) {
        int v = a[i];
        if (v < lo) v = lo;
        else if (v > hi) v = hi;
        total += v;
    }
    return total;
}
