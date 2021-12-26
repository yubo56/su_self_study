#include <cstdio>
int main(int argc, const char *argv[])
{
    FILE *fptr;
    const char *fn = "9test.in";
    int curr;

    fptr = fopen(fn, "r");
    fscanf(fptr, "%d", &curr);
    while (!feof(fptr))
    {
        fscanf(fptr, "%d", &curr);
    }
    return 0;
}
