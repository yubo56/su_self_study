use feature ':5.10';
use strict;
use warnings;

my @reverse_me = split(//, 'not_a_palindrome');
my $sort_me = [5, 3, 6, 0, 4, 2, 7, 9, 8, 1];
my @sort_me_list = (5, 3, 6, 0, 4, 2, 7, 9, 8, 1);

sub reverse_arr
{
    my $arg = shift @_;
    my @list = @{ $arg };
    my @out = ();

    for (my $i = 0; $i < scalar @list; $i++)
    {
        $out[scalar @list - $i - 1] = $list[$i];
    }
    return @out;
}

sub reverse_clever
{
    my $arg = shift @_;
    my @list = @{ $arg };
    my @out = ();

    map({ unshift(@out, $_) } @list);

    return @out;
}

sub realloc_qsort
{
    my @arr = @{ shift @_ };
    my @left;
    my @right;
    my $pivot;

    if (scalar @arr == 0)
    {
        return ();
    }

    $pivot = shift @arr;
    foreach my $val (@arr) {
        if ($val < $pivot)
        {
            push @left, $val;
        }
        else
        {
            push @right, $val;
        }
    }
    return (realloc_qsort(\@left), $pivot, realloc_qsort(\@right));
}

sub inplace_qsort
{
    # cmp pivot w/ pivot+1, swap to end or w/ pivot, end--, continue
    my $arr = shift @_;
    my ($start, $end) = @_;
    my $end_idx = $end;
    my $pivot_idx = $start;
    my $tmp; # swapping

    if ($start >= $end)
    {
        return;
    }

    while ($end_idx > $pivot_idx)
    {
        if ($arr->[$pivot_idx + 1] < $arr->[$pivot_idx])
        {
            $tmp = $arr->[$pivot_idx + 1];
            $arr->[$pivot_idx + 1] = $arr->[$pivot_idx];
            $arr->[$pivot_idx] = $tmp;
            $pivot_idx ++;
        }
        else
        {
            $tmp = $arr->[$end_idx];
            $arr->[$end_idx] = $arr->[$pivot_idx + 1];
            $arr->[$pivot_idx + 1] = $tmp;
            $end_idx --;
        }
    }
    inplace_qsort($arr, $start, $pivot_idx - 1);
    inplace_qsort($arr, $pivot_idx + 1, $end);
}

say reverse_arr(\@reverse_me);
say reverse_clever(\@reverse_me);

say realloc_qsort($sort_me);
say realloc_qsort(\@sort_me_list);

inplace_qsort($sort_me, 0, scalar @{ $sort_me } - 1);
say @{ $sort_me };
