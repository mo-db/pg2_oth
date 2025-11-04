#!/bin/bash

clang++ test.cpp -I./luajit_fin/include/luajit-2.1 \
-L./luajit_fin/lib -lluajit-5.1.2 -o a.out
