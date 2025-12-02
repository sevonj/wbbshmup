#!/bin/sh
set -e

cmake -S . -B ./build
cmake --build ./build
mv -f build/game.linux.template_debug.x86_64.so bin/