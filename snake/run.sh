git clean -dfX
now=$(date +"%T")
echo "Build at time: $now"
cmake ./CMakeLists.txt
cmake --build .
./Snake
