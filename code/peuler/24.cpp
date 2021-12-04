/**
 * What is the millionth permutation of the digits 0-9, in lexicographic order?
 */
#include <cstdio>
const int N = 10;
void print_num(int *digits, int *pr_idx, int len, int idx, int target_pr_idx)
{
   int tmp;
   if (len - idx == 1)
   {
       *pr_idx += 1;
       if (*pr_idx == target_pr_idx)
       {
           for (int i = 0; i < N; i++)
           {
               printf("%d", digits[i]);
           }
           printf("\n");
       }
       return;
   }
   print_num(digits, pr_idx, len, idx + 1, target_pr_idx);
   for (int i = 1; i < len - idx; i++)
   {
       // cycle through digits in lexicographic order
       tmp = digits[idx + i];
       for (int j = idx + i; j > idx; j--)
       {
           digits[j] = digits[j - 1];
       }
       digits[idx] = tmp;
       print_num(digits, pr_idx, len, idx + 1, target_pr_idx);

       tmp = digits[idx];
       for (int j = idx; j < idx + i; j++)
       {
           digits[j] = digits[j + 1];
       }
       digits[idx + i] = tmp;
   }
}
int main(int argc, const char *argv[])
{
    int digits[N];
    int pr_idx = 0;
    for (int i = 0; i < N; i++)
    {
        digits[i] = i;
    }
    print_num(digits, &pr_idx, N, 0, 1e6);
    return 0;
}
