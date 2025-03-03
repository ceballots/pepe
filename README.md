# Pepe

This project provides a Python extension module built using `pybind11` for interacting with a C++ implementation of a key-value store. The extension allows you to use the key-value store directly from Python, providing both an interface to insert data and benchmark its performance.

## Compilation

To compile the C++ code and create the shared library, use the following command:

```bash
c++ -O3 -Wall -shared -std=c++17 -fPIC \
    `python3 -m pybind11 --includes` \
    src/KVStore.cpp -o KVStoreModule.so \
    -undefined dynamic_lookup
```
Then, to run the creation of the index

```bash
python write.py --n 1000000
```

And to run the benchmark

```bash
python benchmark.py --n 1000000
```