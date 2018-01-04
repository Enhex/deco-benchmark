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
serialize: 30.023ms
parse: 15.175ms

JSON
file size: 698835 bytes
serialize: 239.332ms
parse: 338.009ms

XML
file size: 898996 bytes
serialize: 432.129ms
parse: 171.646ms

Deco is:
41.8604% smaller than JSON
797.162% faster output than JSON
2227.41% faster input than JSON
54.8051% smaller than XML
1439.33% faster output than XML
1131.11% faster input than XML
```