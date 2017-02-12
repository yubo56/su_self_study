use feature ':5.10';
use strict;
use warnings;

# The following iterative sequence is defined for the set of positive integers:

# n → n/2 (n is even)
# n → 3n + 1 (n is odd)

# Using the rule above and starting with 13, we generate the following sequence:

# 13 → 40 → 20 → 10 → 5 → 16 → 8 → 4 → 2 → 1
# It can be seen that this sequence (starting at 13 and finishing at 1)
# contains 10 terms. Although it has not been proved yet (Collatz Problem), it
# is thought that all starting numbers finish at 1.

# Which starting number, under one million, produces the longest chain?

# NOTE: Once the chain starts the terms are allowed to go above one million.

sub collatz
{
    if ($_[0] % 2 == 0)
    {
        return $_[0] / 2;
    }
    else
    {
        return 3 * $_[0] + 1;
    }
}

sub max
{
    # @_ is the array we're trying to take max of
    my @arr = @{ shift @_ };
    my $max = $arr[0];
    my $max_idx = 0;
    for (my $idx = 0; $idx < scalar @arr; $idx++)
    {
        if ($arr[$idx] > $max)
        {
            $max = $arr[$idx];
            $max_idx = $idx;
        }
    }
    return ($max, $max_idx);
}

sub solve
{
    my $MAX = shift @_;
    my @hash_arr = (0, 1); # base case hash_arr[1] = 1, hash_arr[0] unused
    my $steps;
    my $curr;
    my @max_info;

    foreach my $val ( 2..$MAX )
    {
        $curr = $val;
        $steps = 0;
        while (!defined $hash_arr[$curr])
        {
            $curr = collatz($curr);
            $steps ++;
        }
        $hash_arr[$val] = $hash_arr[$curr] + $steps;
    }
    @max_info = max(\@hash_arr);
    say $max_info[1] . ' took ' . $max_info[0] . ' steps';
    return $max_info[1];
}

solve(1e6 - 1);
