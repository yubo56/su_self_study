# Substring Search

Implementing some algs for substring search for fun.

### TODO
- Boyer-Moore

### Notes

- Naive actually does shockingly well when it's not on worst case performance
    - Worst case = searching for a^mb in a^n
- Ran on a corpus with `wc` stats of `871192 805007 38207393 corpus.txt`, containing all 'a's
- Runtimes with "a^154b" look something like
    - Naive: ~= 4.71 +- 0.04s
    - RK: ~= 0.74 +- 0.02s
    - KMP: ~= 0.58 +- 0.01s
