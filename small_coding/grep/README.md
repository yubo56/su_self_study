# Substring Search

Implementing some algs for substring search for fun.

### TODO
- Boyer-Moore

### Notes

- Naive actually does shockingly well when it's not on worst case performance
    - Worst case = searching for a^mb in a^n
- Runtimes on a corpus of all of Shakespeare with 's/./a/g' with "a^142b" look
  something like
    - Naive: ~= 0.63 +- 0.05s
    - RK: ~= 0.11 +- 0.02s
    - KMP: ~= 0.10 +- 0.02s
