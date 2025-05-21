#!/bin/bash

set -e

mkdir -p obj

g++ -std=c++17 -Wall -Wextra -I./include -c src/*.cpp  -w

mv *.o obj/

g++ obj/*.o -o reward_wallet

./reward_wallet