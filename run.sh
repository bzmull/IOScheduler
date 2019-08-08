#!/usr/bin/env bash

make
cd ./inputs/
bash ./runit.sh ../my_outputs/ ../iosched
bash ./gradeit.sh ../refout/ ../my_outputs/
cd ..
make clean