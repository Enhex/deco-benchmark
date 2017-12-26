cd ..
mkdir deco-benchmark-build
cd deco-benchmark-build

conan install ../deco-benchmark --build=missing
cmake ../deco-benchmark -G "Visual Studio 15 Win64"