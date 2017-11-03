cd ..
mkdir deco-benchmark-build
cd deco-benchmark-build

conan install ../deco-benchmark
conan install ../deco-benchmark -s build_type=Debug
cmake ../deco-benchmark -G "Visual Studio 15 Win64"