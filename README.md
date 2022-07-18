# Framework
A framework made for usage in internal training softwares under GNU/Linux.  

### Compilation
Optimization can be used, but due to the self-modifying-code in the return address spoofer, it is locked to O0.  

### Hooking
Because writing a disassembler was not my goal for this project, a `instructionLength` parameter was intoduced.  
It describes the amount of bytes which are supposed to be copied into the proxy.

### Return Address Spoofing
Return Address Spoofing uses a dynarec-like approach and thus is a hell for the gcc-optimizer.  
All methods have received anti-optimization attributes respectively.  
Before using the Spoofer a ``return_instruction_addr`` variable has to be set.  
More details in the respective file and in Tests.cpp  

### Usage
This framework can be implemented in multiple ways.  
The easiest is probably to use a git submodule. 
The Source/Tests.cpp contains a example for every tool
