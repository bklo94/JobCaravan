#!/bin/bash
make clean
make all -i > build.log 2>&1
./bin/main > output.log 2>&1
([ $? -eq 0 ] && echo "Success!") || echo "Failure!"
