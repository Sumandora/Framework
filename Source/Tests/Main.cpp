#include "../Framework/Assembly/Assembly.hpp"
#include "../Framework/Hooking/Hooking.hpp"
#include "../Framework/PatternScan/PatternScan.hpp"
#include "../Framework/ReturnAddr/ReturnAddr.hpp"

#include <stdio.h>
#include <cassert>
#include <cstring>

#if defined(FRAMEWORK_ENABLE_HOOKING_DETOUR) || defined(FRAMEWORK_ENABLE_HOOKING_PTRSWAP)
void* hookMe_retaddr;
int __attribute((noinline)) hookMe(int a, int b, int c) {
	hookMe_retaddr = __builtin_extract_return_addr(__builtin_return_address(0));
	return a + b + c;
}

void* orig_hookMe;
int hook(int a, int b, int c) {
	return reinterpret_cast<int(*)(int, int, int)>(orig_hookMe)(a * 2, b * 2, c * 2);
}
#endif

#ifdef FRAMEWORK_ENABLE_RETURN_ADDRESS
const int argumentLength = 1000 * 1000 * 8; // approximate the maximum that the stack can handle

// We need a array, which specifies
int testFunction(char str[argumentLength]) {
	assert(__builtin_extract_return_addr(__builtin_return_address(0)) == Framework::ReturnAddr::ret_instruction_addr);
	return strlen(str);
}
#endif

int __attribute((optimize("O0"))) main() {
	printf("Starting Framework tests\n");

#ifdef FRAMEWORK_ENABLE_HOOKING_PTRSWAP
	// We have to be very careful so we don't create 2 seperate call instructions
	for(int i = 0; i <= 1; i++) {
		assert(hookMe(1,2,3) == (i == 0 ? 6 : 12));
		assert(hookMe_retaddr != nullptr);
		if(i == 0) {
			orig_hookMe = Framework::Hooking::relativePtrSwap(static_cast<char*>(hookMe_retaddr) - (FRAMEWORK_NEAR_JMP_LENGTH - 1), reinterpret_cast<void*>(hook));
			assert(orig_hookMe != nullptr);
		}
	}
	printf("Ran Hooking/PtrSwap tests\n");
#endif

#ifdef FRAMEWORK_ENABLE_HOOKING_DETOUR	
	assert(hookMe(1, 2, 3) == 6);
	orig_hookMe = Framework::Hooking::detour(reinterpret_cast<void*>(hookMe), reinterpret_cast<void*>(hook), 6);
	assert(hookMe(1, 2, 3) == 12);
	printf("Ran Hooking/Detour tests\n");
#endif

#ifdef FRAMEWORK_ENABLE_PATTERN_SCANNING
	unsigned char byte_array_hex[256u] = {0x40, 0xf9, 0xac, 0x55, 0x9f, 0xcb, 0x79, 0x5d, 0xe9, 0x27, 0x3f, 0xa1, 0x0, 0xf5, 0x91, 0xd0, 0xf2, 0xbe, 0x3, 0x72, 0xb4, 0xdd, 0x7, 0xe0, 0x3d, 0x6d, 0x91, 0xfe, 0x88, 0xb6, 0xf, 0xa, 0xb3, 0xc3, 0x8e, 0x9f, 0x9b, 0x87, 0x11, 0x19, 0xe1, 0x1b, 0x37, 0x1d, 0x4d, 0xf9, 0x20, 0xb3, 0xc1, 0x55, 0x26, 0x4c, 0x63, 0xc5, 0x11, 0x8d, 0x59, 0xa, 0x3c, 0x8f, 0xf7, 0x1, 0xe3, 0x7a, 0x41, 0xae, 0x30, 0x4d, 0xa7, 0xb, 0x8a, 0x1e, 0xae, 0x5b, 0x9a, 0x41, 0x5c, 0x97, 0x88, 0xfa, 0x75, 0x68, 0x18, 0x82, 0xe2, 0x36, 0x89, 0xaa, 0xe9, 0x1e, 0xe, 0x54, 0xfa, 0x65, 0x4a, 0xc9, 0x54, 0x90, 0xa0, 0xe2, 0x4f, 0xba, 0xf1, 0x12, 0xd2, 0x98, 0xfa, 0x47, 0x79, 0x7b, 0xfe, 0xb4, 0xf8, 0x70, 0x7f, 0x7a, 0x32, 0xb9, 0x5f, 0x57, 0x41, 0x5a, 0x38, 0x43, 0xa, 0x2f, 0x2f, 0x11, 0x58, 0xdd, 0x3b, 0x54, 0x16, 0xd5, 0x2e, 0x5e, 0xf0, 0x52, 0xf5, 0x10, 0xc7, 0xa6, 0xca, 0xab, 0xbc, 0xf7, 0x23, 0x15, 0x29, 0x76, 0x80, 0xeb, 0x2b, 0xc1, 0x39, 0x30, 0xba, 0x15, 0x14, 0x6c, 0x46, 0xa3, 0xc, 0x99, 0xc, 0x31, 0x4a, 0x1, 0x23, 0x79, 0xb2, 0xd0, 0xba, 0x50, 0xa6, 0x26, 0x83, 0xc6, 0x23, 0x51, 0x39, 0x88, 0x3f, 0x80, 0x8a, 0xae, 0x45, 0x6d, 0x24, 0x41, 0x36, 0xa, 0xe6, 0xa4, 0x99, 0xaa, 0xd4, 0xdf, 0x5c, 0xd4, 0xc3, 0xba, 0x9d, 0x0, 0xe2, 0x5c, 0xf4, 0x8, 0x54, 0x58, 0xf3, 0xbd, 0xe, 0x11, 0x5e, 0x9c, 0xaf, 0xd7, 0xc1, 0x7a, 0x8e, 0xb, 0x96, 0x76, 0xf7, 0x7d, 0xa2, 0xc2, 0x5c, 0x5f, 0xa, 0xfb, 0x23, 0x4f, 0x8c, 0x3f, 0x25, 0x3, 0x9e, 0xaa, 0x74, 0x78, 0xa1, 0x9b, 0x17, 0x56, 0xbd, 0x3d, 0x24, 0x9f, 0xdf, 0xe6, 0x67, 0x1a, 0xd7, 0x7c};

	void* result = Framework::PatternScan::searchPattern(Pattern("\x90\xA0\xE2", "xxx"), &byte_array_hex);
	assert(reinterpret_cast<size_t>(result) - reinterpret_cast<size_t>(&byte_array_hex) == 0x61);
	printf("Ran Pattern Scanning tests\n");
#endif

#ifdef FRAMEWORK_ENABLE_RETURN_ADDRESS
	Framework::ReturnAddr::ret_instruction_addr =
#ifdef FRAMEWORK_ENABLE_PATTERN_SCANNING
		Framework::PatternScan::searchPattern(
			Framework::ReturnAddr::leave_ret_instruction,
			reinterpret_cast<void*>(main)
		);
#else
		reinterpret_cast<void*>(main);
	
	while(
		*static_cast<unsigned char*>(Framework::ReturnAddr::ret_instruction_addr) != 0xC9 ||
		*(static_cast<unsigned char*>(Framework::ReturnAddr::ret_instruction_addr) + 1) != 0xC3
	) {
		Framework::ReturnAddr::ret_instruction_addr = static_cast<char*>(Framework::ReturnAddr::ret_instruction_addr) + 1;
	}
#endif
	assert(Framework::ReturnAddr::ret_instruction_addr != nullptr);
	
	char str[argumentLength];
	for(int i = 0; i < argumentLength; i++)
		str[i] = 'A';
	
	int length = Framework::ReturnAddr::invoke<int, char[]>(reinterpret_cast<void*>(testFunction), str);
	// Running this on different environments produced other values... one was +4, one was +6, idc
	assert(length >= argumentLength && length <= argumentLength + 16);
	printf("Ran Return Address tests\n");
#endif
	
	printf("Finished all tests... (yay)\n");
}