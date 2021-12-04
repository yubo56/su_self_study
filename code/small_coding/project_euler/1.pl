use feature ':5.10';
use strict;
use warnings;

# If we list all the natural numbers below 10 that are multiples of 3 or 5, we
# get 3, 5, 6 and 9. The sum of these multiples is 23. Find the sum of all the
# multiples of 3 or 5 below 1000.

my $upper = 1000;
my $sum = 0;

# iterative version
my @divisibles = grep { $_ % 3 == 0 || $_ % 5 == 0 } ( 1.. $upper - 1 );
foreach my $num ( @divisibles )
{
    $sum += $num;
}

say $sum;


# List::Util version

use List::Util qw(reduce);
say reduce { $a += $b } grep { $_ % 3 == 0 || $_ % 5 == 0 } ( 1.. $upper - 1 );
