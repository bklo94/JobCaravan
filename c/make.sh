#!/bin/bash
make clean
make all -i > build.log 2>&1
./bin/server > output.log 2>&1
([ $? -eq 0 ] && echo "Success!") || echo "Failure!"
