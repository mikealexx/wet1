#!/bin/bash

for i in {0..29}; do
  if diff randtest$i.out test$i.out > /dev/null; then
    echo "test $i passed"
  else
    echo "test $i failed"
  fi
done