#!/bin/sh

g++ -Wall -I./include -c named_array.cpp
g++ -I./include -c mam_trees.cpp
g++ -I./include -c tree_wrap.cpp

ar -r libarbowrap.a named_array.o mam_trees.o tree_wrap.o


