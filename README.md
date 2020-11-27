
# DT5550 C++ Citiroc Decode Function
This C++ code allows to decode .data file saved by DT5550W board in order to import them in software like root.
This is not a library but a piece of code that can be integrated into the user final application.
The output of the decode is a textual file but can be easy replaced with a vector of class including the decoded data.
## Windows compiling
Just open the Visual Studio 2015 project and compile it.

## Linux Compiling
Install the g++ compiler and type

```bash
g++ CitirocBinaryDecode.cpp -o CitirocBinaryDecode
```

