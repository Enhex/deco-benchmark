# Deco Benchmark

Benchmarks Deco againt JSON and XML, using [Cereal library](https://uscilab.github.io/cereal/) which internally uses RapidJSON and RapidXML.

The benchmark generates objects with random content with up to 3 levels of indented nesting.

Input/Output measures serialization from/to in memory string, excluding file I/O.


## Results

Setup:
- OS: Windows 10 64-bit
- Compiler: Visual Studio 15.5, x64 build
- CPU: i7-6700K

Output:
```
Deco
file size: 406300 bytes
serialize: 30.576ms
parse: 15.797ms

JSON
file size: 698835 bytes
serialize: 239.433ms
parse: 336.594ms

XML
file size: 898996 bytes
serialize: 425.198ms
parse: 171.65ms

Deco is:
41.8604% smaller than JSON
783.075% faster output than JSON
2130.75% faster input than JSON
54.8051% smaller than XML
1390.63% faster output than XML
1086.6% faster input than XML
```