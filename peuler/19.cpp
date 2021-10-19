/**
 * How many Sundays fell on the first of the month during the 20th century
 * (1/1/1901-12/31/2000)? 1/1/1900 was a Monday
 *
 * Literally the worst...
 */
#include <cstdio>
int main(int argc, const char *argv[])
{
    int num_sundays = 0;
    int dow = 1; // Sunday = 0

    int month = 1;
    int year = 1900;
    while (year < 2001)
    {
        if (dow == 0 && year >= 1901)
        {
            num_sundays++;
        }
        if (month == 2)
        {
            if (year % 4 == 0)
            {
                if (year % 100 == 0)
                {
                    if (year % 400 == 0)
                    {
                        dow += 29;
                    }
                    else
                    {
                        dow += 28;
                    }
                }
                else
                {
                    dow += 29;
                }
            }
            else
            {
                dow += 28;
            }
        }
        else if (month == 4 || month == 6 || month == 9 || month == 11)
        {
            dow += 30;
        }
        else
        {
            dow += 31;
        }
        month += 1;
        dow %= 7;
        if (month > 12)
        {
            month = 1;
            year++;
        }
        // printf("%d / %d (%d)\n", month, year, dow);
    }
    printf("Num Sundays: %d\n", num_sundays);
    return 0;
}
