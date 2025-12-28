mkdir build
cd build
cmake ..
cmake --build . --config Debug
ctest -C Debug --output-on-failure
pause