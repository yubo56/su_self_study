/**
 * Proof: Given X_n, find Y such that:
 *
 * (i) sum |X_n - Y| is minimized: this can be rewritten Sum_{X_n < Y} (Y - X_n)
 *      + Sum_{X_N > Y} (X_n - Y). Any change in Y causes the original sum to
 *      change by the difference in *number* of elements on either side of Y. It
 *      is then clear that if Y is the median, we have a local extremum, and
 *      inspection shows that this is global minimum.
 *
 * (ii) sum (X_n - Y)^2: This is even easier, we can differentiate (continuous
 *      objective function) to find the minmimum: sum(Y - X_n) = 0, so sum(X_n)
 *      = sum(Y), and X_n is the mean.
 *
 *      NB: I just used sum of square, but it's really (X_n - Y) * (X_n - Y +-
 *      1) / 2, or (X_n - Y)^2 + abs(X-n - Y), so it's between the mean and
 *      median. Maybe just linear search from mean is fastest?
 */
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>
const int N = 1000; // num crabs
int main(int argc, const char *argv[])
{
    FILE *fptr;
    const char *fn = "7.in";
    std::vector<int> locs;
    int curr = 0;

    fptr = fopen(fn, "r");
    fscanf(fptr, "%d", &curr);
    locs.push_back(curr);
    while (fscanf(fptr, ",%d", &curr))
    {
        locs.push_back(curr);
    }

    // Part 1: this should be either mean or median right? Median: if 1, 1, 1,
    // 1000, minimum distance is at 1, not 253 or w/e
    std::sort(locs.begin(), locs.end());
    int sum_dist = 0;
    int median = locs[locs.size() / 2];
    for (auto i = locs.begin(); i < locs.end(); i++)
    {
        sum_dist += abs(*i - median);
    }
    printf("Sum distances is %d\n", sum_dist);

    // Part 2: sum of distances squared, is mean? See head of doc
    int sum = std::accumulate(locs.begin(), locs.end(), 0);
    int prev_min = -1;
    int mean = round(1.0 * sum / locs.size());
    int inc = (median - mean) / abs(mean - median);
    while (true)
    {
        int sum_dist_sq = 0;
        for (auto i = locs.begin(); i < locs.end(); i++)
        {
            sum_dist_sq += abs(*i - mean) * (abs(*i - mean) + 1) / 2;
        }
        // printf("Sum distances squared (from mean %d) is %d\n",
        //         mean, sum_dist_sq);
        if (prev_min > 0 && sum_dist_sq > prev_min) break;
        prev_min = sum_dist_sq;
        mean += inc;
    }
    printf("Sum distances squared is %d\n", prev_min);
    return 0;
}
/*
--- Day 7: The Treachery of Whales ---

A giant whale has decided your submarine is its next meal, and it's much faster than you are. There's nowhere to run!

Suddenly, a swarm of crabs (each in its own tiny submarine - it's too deep for them otherwise) zooms in to rescue you! They seem to be preparing to blast a hole in the ocean floor; sensors indicate a massive underground cave system just beyond where they're aiming!

The crab submarines all need to be aligned before they'll have enough power to blast a large enough hole for your submarine to get through. However, it doesn't look like they'll be aligned before the whale catches you! Maybe you can help?

There's one major catch - crab submarines can only move horizontally.

You quickly make a list of the horizontal position of each crab (your puzzle input). Crab submarines have limited fuel, so you need to find a way to make all of their horizontal positions match while requiring them to spend as little fuel as possible.

For example, consider the following horizontal positions:

16,1,2,0,4,2,7,1,2,14

This means there's a crab with horizontal position 16, a crab with horizontal position 1, and so on.

Each change of 1 step in horizontal position of a single crab costs 1 fuel. You could choose any horizontal position to align them all on, but the one that costs the least fuel is horizontal position 2:

    Move from 16 to 2: 14 fuel
    Move from 1 to 2: 1 fuel
    Move from 2 to 2: 0 fuel
    Move from 0 to 2: 2 fuel
    Move from 4 to 2: 2 fuel
    Move from 2 to 2: 0 fuel
    Move from 7 to 2: 5 fuel
    Move from 1 to 2: 1 fuel
    Move from 2 to 2: 0 fuel
    Move from 14 to 2: 12 fuel

This costs a total of 37 fuel. This is the cheapest possible outcome; more expensive outcomes include aligning at position 1 (41 fuel), position 3 (39 fuel), or position 10 (71 fuel).

Determine the horizontal position that the crabs can align to using the least fuel possible. How much fuel must they spend to align to that position?

Your puzzle answer was 329389.

The first half of this puzzle is complete! It provides one gold star: *
--- Part Two ---

The crabs don't seem interested in your proposed solution. Perhaps you misunderstand crab engineering?

As it turns out, crab submarine engines don't burn fuel at a constant rate. Instead, each change of 1 step in horizontal position costs 1 more unit of fuel than the last: the first step costs 1, the second step costs 2, the third step costs 3, and so on.

As each crab moves, moving further becomes more expensive. This changes the best horizontal position to align them all on; in the example above, this becomes 5:

    Move from 16 to 5: 66 fuel
    Move from 1 to 5: 10 fuel
    Move from 2 to 5: 6 fuel
    Move from 0 to 5: 15 fuel
    Move from 4 to 5: 1 fuel
    Move from 2 to 5: 6 fuel
    Move from 7 to 5: 3 fuel
    Move from 1 to 5: 10 fuel
    Move from 2 to 5: 6 fuel
    Move from 14 to 5: 45 fuel

This costs a total of 168 fuel. This is the new cheapest possible outcome; the old alignment position (2) now costs 206 fuel instead.

Determine the horizontal position that the crabs can align to using the least fuel possible so they can make you an escape route! How much fuel must they spend to align to that position?

Answer:

Although it hasn't changed, you can still get your puzzle input.

You can also [Shareon Twitter Mastodon] this puzzle.
 */
