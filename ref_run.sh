#!/usr/bin/env bash


make
cd ./inputs/
bash ./runit.sh ../trace_outputs ../refiosched -v
./runit.sh ../my_outputs/ ../iosched -v
./gradeit.sh ../trace_outputs/ ../my_outputs/
cd ..
make clean
