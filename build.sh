#!/bin/bash +xe

g++ \
*.cpp \
windows/*.cpp \
-o passtis \
-lyaml-cpp \
-lcrypto \
-lncurses \
-lform \
-Wall
