
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

# New firmware file format

Firmware and software > 2021.12.6 added a validation data counter (see manual) to the data stored on the files. 
YOU MUST set to 1 the define `#define FW_USE_VALIDATION 0` in order to be able to decode data 

