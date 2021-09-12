#!/bin/bash

for changed_file in $(git diff --name-only ; git ls-files --others --exclude-standard)
do
    if [[ $changed_file =~ .*\.+(cpp|cc|cxx|c|hpp|h)$ ]]; then
        path='./'$changed_file
        echo $path
        clang-format -i -style=file $path
    fi
done
