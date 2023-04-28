# Framework
A framework made for usage in internal training softwares under GNU/Linux.

## Deprecation
### This project is deprecated. For improved alternatives check out these projects:
- https://github.com/Sumandora/DetourHooking
- https://github.com/Sumandora/ReturnAddressSpoofer
- https://github.com/Sumandora/SignatureScanner

### Compilation
Optimization can be used, but due to the self-modifying-code in the return address spoofer, the return address spoofer can't be optimized.

### Hooking
Because writing a disassembler was not my goal for this project, a `instructionLength` parameter was intoduced.  
It describes the amount of bytes which are supposed to be copied into the proxy.  
Pointer Swapping works well and should be used in order to bypass anticheats, which simply check whether or not the opcodes from disk match

### Return Address Spoofing
Return Address Spoofing uses a self-modifying code approach and thus is a hell for the gcc-optimizer.  
All methods have received anti-optimization attributes respectively.  

### Usage
This framework can be implemented in multiple ways.  
The easiest is probably to use a git submodule.  
The Source/Tests.cpp contains a example for every tool
