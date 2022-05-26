# Framework
A framework made for usage in internal training softwares under GNU/Linux.

### Compilation
Optimization can be used, but will lead to a segfault in the Tests.cpp file under gcc 12.1.0, because gcc does not predict the .text section patches which lead to another execution flow, than previously anticipated by gcc.

### Hooking
Because writing a disassembler was not my goal for this project, a `instructionLength` parameter was intoduced.  
It describes the amount of bytes which are supposed to be copied into the proxy.

### Return Address Spoofing
Return Address Spoofing uses a dynarec-like approach and thus is a hell for the gcc-optimizer.  
All methods have received anti-optimization attributes respectively.

### Usage
This framework can be implemented in multiple ways.  
The easiest is probably to use a git submodule.  
