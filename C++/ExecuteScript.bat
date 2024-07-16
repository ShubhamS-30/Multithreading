@echo off
echo --- Making Build Directory ---
mkdir Build
echo --- BUILDING PROJECT ---
cmake -B .\Build\
echo --- RELEASING DEBUG MODE ---
cmake --build .\Build\ 
echo --- EXECUTING RELEASE ---
.\Build\Debug\MULTITHREADING.exe
echo --- EXECUTION COMPLETE ---
