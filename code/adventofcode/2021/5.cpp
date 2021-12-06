#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
// const int N = 10;
// const char *fn = "5test.in";
const int N = 1000;
const char *fn = "5.in";

void fill_grid(int *grid, int x1, int y1, int x2, int y2, bool ignore_diag)
{
    int fillx = x1;
    int filly = y1;
    if (ignore_diag && x1 != x2 && y1 != y2)
    {
        return;
    }
    // printf("Filling (%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
    int incx = x2 == x1 ? 0 : (x2 - x1) / abs(x2 - x1);
    int incy = y2 == y1 ? 0 : (y2 - y1) / abs(y2 - y1);
    grid[filly * N + fillx]++;
    while (fillx != x2 || filly != y2)
    {
        fillx += incx;
        filly += incy;
        grid[filly * N + fillx]++;
    }
}
int main(int argc, const char *argv[])
{
    FILE *fptr;
    int x1, y1, x2, y2;
    int *grid = (int *)malloc(N * N * sizeof(int));
    int *grid_diag = (int *)malloc(N * N * sizeof(int));
    memset(grid, 0, N * N * sizeof(int));

    fptr = fopen(fn, "r");
    fscanf(fptr, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2);
    while (!feof(fptr))
    {
        fill_grid(grid, x1, y1, x2, y2, true);
        fill_grid(grid_diag, x1, y1, x2, y2, false);
        fscanf(fptr, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2);
    }

    int count = 0;
    int count_diag = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // printf("%d", grid[i * N + j]);
            if (grid[i * N + j] > 1)
            {
                count++;
            }
            if (grid_diag[i * N + j] > 1)
            {
                count_diag++;
            }
        }
        // printf("\n");
    }
    printf("%d points with >2 lines\n", count);
    printf("%d points with >2 lines, counting diagonals\n", count_diag);
    free(grid);
    return 0;
}
