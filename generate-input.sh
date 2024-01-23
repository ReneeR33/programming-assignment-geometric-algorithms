#!/bin/bash

GENERATOR="ranGenerater"

rm -rf input/${GENERATOR}
mkdir input/${GENERATOR}

for i in {1..100}
do
   n=$((${i} * 1000))
   echo "generating input for n = $n"
   python3 input-generators/${GENERATOR}.py $n >> input/${GENERATOR}/input-${n}.txt
done

echo "Testing runtime for plane sweep"
python3 runtime-tester.py input/${GENERATOR} build/plane-sweep;

echo "Testing runtime for brute force"
python3 runtime-tester.py input/${GENERATOR} brute-force/brute-force;