/**
 * Sort the list of names, then obtain the sum of the products of the
 * alphabetical value of each name (sum of characters, a = 1) by its rank
 */
#include <cstdio>
#include <cstring>
const int NAME_LEN = 15; // upper bound
const int NUM_NAMES = 5163; // upper bound (5163, to be exact)
int to_int(char a)
{
    return a - 64; // A = 1
}
int strcmp(const char *a, const char *b)
{
    while (*a != 0 && *b != 0 && to_int(*a) == to_int(*b))
    {
        a++;
        b++;
    }
    return *a - *b;
}
void qsort(char *names, int *name_idxs, int left, int right)
{
    int tmp;
    int pivot = left;
    int end = right - 1;
    if (end - pivot <= 0) { return ; }

    while (end - pivot > 0)
    {
        if (strcmp(names + name_idxs[pivot], names + name_idxs[pivot + 1]) < 0)
        {
            tmp = name_idxs[pivot + 1];
            name_idxs[pivot + 1] = name_idxs[end];
            name_idxs[end] = tmp;
            end--;
        }
        else
        {
            tmp = name_idxs[pivot + 1];
            name_idxs[pivot + 1] = name_idxs[pivot];
            name_idxs[pivot] = tmp;
            pivot++;
        }
    }
    qsort(names, name_idxs, left, pivot);
    qsort(names, name_idxs, pivot + 1, right);
}
int name_val(char *s)
{
    int sum = 0;
    while (*s != 0)
    {
        sum += to_int(*s);
        s++;
    }
    return sum;
}
int main(int argc, const char *argv[])
{
    FILE *fptr;
    const char* fn = "22_names.txt";
    char names[NAME_LEN * NUM_NAMES];
    int name_idxs[NUM_NAMES];
    memset(names, 0, sizeof(names));
    memset(name_idxs, 0, sizeof(name_idxs));

    fptr = fopen(fn, "r");
    fscanf(fptr, "%s", names);

    name_idxs[0] = 1;
    int num_names = 1;
    for (int i = 0; i < NAME_LEN * NUM_NAMES; i++)
    {
        if (names[i] == 0) { break; }
        if (names[i] == ',')
        {
            name_idxs[num_names] = i + 2;
            num_names++;
        }
        else if (names[i] == '\"')
        {
            names[i] = 0;
        }
    }
    // names + name_idxs[i] = i-th name, now time to quicksort
    qsort(names, name_idxs, 0, num_names);
    long name_sum = 0;
    for (int i = 0; i < num_names; i++)
    {
        // printf("%s, %d\n", names + name_idxs[i], name_val(names + name_idxs[i]));
        name_sum += (i + 1) * name_val(names + name_idxs[i]);
    }
    printf("Sum is %ld\n", name_sum);
    return 0;
}
