#!/usr/bin/env bash

if [[ $# < 1 ]]; then
    echo "Usage: $0 num_of_tests"
    exit 1
fi

pass=true
for ((i=1;i<=$1;i++)); do
    iter_pass=true
    for file in  "in$i.txt" "out$i.txt"; do
        if [[ ! -f "$file" ]]; then
            echo Missing "$file"
            pass=false
            iter_pass=false
        fi
    done
    if [[ $iter_pass == true ]]; then
        ./chess < "in$i.txt" > out.txt
        diff=$(diff out.txt out$i.txt)
        if [[ "$diff" != "" ]]; then
            echo "FAILED test $i!"
            pass=false
         fi
    fi
done

if [[ $pass == true ]]; then
    echo "PASSED all tests"
fi
