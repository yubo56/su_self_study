#!/bin/bash
# lists all modules we can run from the current directory, as well as the
# second lines of the files: first line is shebang, second describes purpose

for i in $(find 161 -iname *.py)
    do echo $i | sed 's/\//./g; s/\.py//g'
    printf "\t$(cat $i | head -n 2 | tail -n 1 | sed 's/^#\ //g')\n"
done
