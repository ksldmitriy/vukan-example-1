#!/bin/bash

rm debug-logs
rm info-logs

echo === CMAKE ===

cmake -S ./src -B ./out  

mv out/compile_commands.json .

echo === BUILD ===
make -j 16 -C ./out;

echo === SHADERS ===
#glslc shaders/test.comp -o shaders/test.spv
#glslc shaders/test.vert -o shaders/test_v.spv
#glslc shaders/test.frag -o shaders/test_f.spv

echo === RUN ===
./out/best_program

