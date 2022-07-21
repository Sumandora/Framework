//
// Created by johannes on 4/14/22.
//
#include <iostream>
#include <cassert>
#include <dlfcn.h>
#include <cstring>

#include "framework/Hooking.h"
#include "framework/PatternScan.h"
#include "framework/ReturnAddr.h"

#if defined(ENABLE_RETURN_ADDRESS) || defined(ENABLE_HOOKING_DETOUR)
struct test_struct {
	int a;
	int b;
	long long int c;
};

test_struct testStruct{};
#endif

#ifdef ENABLE_RETURN_ADDRESS
int no_args() {
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

int one_arg(test_struct a) {
	assert(a.a == 1);
	assert(a.b == 2);
	assert(a.c == 3);
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

int two_args(short a, int b) {
	assert(a == 1);
	assert(b == 2);
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

int three_args(short a, int b, long c) {
	assert(a == 1);
	assert(b == 2);
	assert(c == 3);
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

int four_args(short a, int b, long c, unsigned int d) {
	assert(a == 1);
	assert(b == 2);
	assert(c == 3);
	assert(d == 4);
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

int five_args(short a, int b, long c, unsigned int d, long long int e) {
	assert(a == 1);
	assert(b == 2);
	assert(c == 3);
	assert(d == 4);
	assert(e == 5);
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

int six_args(short a, int b, long c, unsigned int d, long long int e, test_struct f) {
	assert(a == 1);
	assert(b == 2);
	assert(c == 3);
	assert(d == 4);
	assert(e == 5);
	assert(f.a == 1);
	assert(f.b == 2);
	assert(f.c == 3);
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

int seven_args(short a, int b, long c, unsigned int d, long long int e, short f, int g) {
	assert(a == 1);
	assert(b == 2);
	assert(c == 3);
	assert(d == 4);
	assert(e == 5);
	assert(f == 6);
	assert(g == 7);
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

int eight_args(short a, int b, long c, unsigned int d, long long int e, short f, int g, long h) {
	assert(a == 1);
	assert(b == 2);
	assert(c == 3);
	assert(d == 4);
	assert(e == 5);
	assert(f == 6);
	assert(g == 7);
	assert(h == 8);
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

int nine_args(short a, int b, long c, unsigned int d, long long int e, short f, int g, long h, unsigned int i) {
	assert(a == 1);
	assert(b == 2);
	assert(c == 3);
	assert(d == 4);
	assert(e == 5);
	assert(f == 6);
	assert(g == 7);
	assert(h == 8);
	assert(i == 9);
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

int ten_args(short a, int b, long c, unsigned int d, long long int e, short f, int g, long h, unsigned int i,
			 long long int j) {
	assert(a == 1);
	assert(b == 2);
	assert(c == 3);
	assert(d == 4);
	assert(e == 5);
	assert(f == 6);
	assert(g == 7);
	assert(h == 8);
	assert(i == 9);
	assert(j == 10);
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}
#endif

// This method has to stay
#if defined (ENABLE_RETURN_ADDRESS) || defined (ENABLE_HOOKING_DETOUR)
int eleven_args(short a, int b, long c, unsigned int d, long long int e, short f, int g, long h, unsigned int i,
				long long int j, test_struct k) {
	assert(a == 1);
	assert(b == 2);
	assert(c == 3);
	assert(d == 4);
	assert(e == 5);
	assert(f == 6);
	assert(g == 7);
	assert(h == 8);
	assert(i == 9);
	assert(j == 10);
	assert(k.a == 1);
	assert(k.b == 2);
	assert(k.c == 3);
#ifdef ENABLE_RETURN_ADDRESS
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
#endif
	return 1337;
}
#endif

#ifdef ENABLE_RETURN_ADDRESS
void test2() {
	
	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int, long>(reinterpret_cast<void*>(eight_args), 1, 2, 3, 4, 5, 6, 7, 8)) == 1337);
	}
	printf("eight_args test passed\n");

	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int, long, unsigned int>(reinterpret_cast<void*>(nine_args), 1, 2, 3, 4, 5, 6, 7, 8, 9)) == 1337);
	}
	printf("nine_args test passed\n");
}

void test() {
	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, test_struct>(reinterpret_cast<void*>(six_args), 1, 2, 3, 4, 5, testStruct)) == 1337);
	}
	printf("six_args test passed\n");

	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int>(reinterpret_cast<void*>(seven_args), 1, 2, 3, 4, 5, 6, 7)) == 1337);
	}
	printf("seven_args test passed\n");

	test2();
}
#endif

#ifdef ENABLE_HOOKING_DETOUR
void* orig_eleven_args;

int eleven_args_hook(short a, int b, long c, unsigned int d, long long int e, short f, int g, long h, unsigned int i,
				long long int j, test_struct k) {
#ifdef ENABLE_RETURN_ADDRESS
	return Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int, long, unsigned int, long long int, test_struct>(reinterpret_cast<void*>(orig_eleven_args), a, b, c, d, e, f, g, h, i, j, k) + 1;
#else
	return reinterpret_cast<int(*)(short, int, long, unsigned int, long long int, short, int, long, unsigned int, long long int, test_struct)>(orig_eleven_args)(a, b, c, d, e, f, g, h, i, j, k) + 1;
#endif
}
#endif

#ifdef ENABLE_HOOKING_PTRSWAP
void* hookMe_retaddr;
int hookMe(int a, int b, int c) {
	hookMe_retaddr = __builtin_return_address(0);
	return a + b + c;
}

void* orig_hookMe;
int __attribute((optimize("O0"))) hook(int a, int b, int c) {
	return reinterpret_cast<int(*)(int, int, int)>(orig_hookMe)(a * 2, b * 2, c * 2);
}
#endif

/*
 * This method has to be kept at O0, because if anything else is used it will crash if you try to reuse the invokers from the Return Address Spoofer
 */
int __attribute((optimize("O0"))) main() {
	printf("Starting tests\n");
	
	void* ret_instruction_addr =
#ifdef ENABLE_PATTERN_SCANNING
		Framework::PatternScan::searchPattern(
#ifdef ENABLE_RETURN_ADDRESS
			Framework::ReturnAddr::leave_ret_instruction,
#else
			Pattern("\xC9\xC3", "xx"),
#endif
			reinterpret_cast<void*>(main)
		);
#else
		reinterpret_cast<void*>(main);
	
	while(
		*static_cast<unsigned char*>(ret_instruction_addr) != 0xC9 ||
		*(static_cast<unsigned char*>(ret_instruction_addr) + 1) != 0xC3
	) {
		ret_instruction_addr = static_cast<char*>(ret_instruction_addr) + 1;
	}
#endif
	assert(ret_instruction_addr != nullptr);
	printf("Found ret_instruction at %p\n", ret_instruction_addr);
	
#ifdef ENABLE_RETURN_ADDRESS
	Framework::ReturnAddr::ret_instruction_addr = ret_instruction_addr;
#endif

#ifdef ENABLE_HOOKING_DETOUR
	/*
	 * We need 5 bytes to setup a hook.
	 * The last argument (8) tells the hooking function to copy 8 bytes.
	 * This was chosen, because this approach does not need a disassembler engine.
	 * 
	 * <+0>:	push   %rbp
	 * <+1>:	mov	%rsp,%rbp
	 * <+4>:	sub	$0x50,%rsp
	 * <+8>:	mov	%edi,%eax
	 * 
	 * In case you are optimizing using O2, it will look like this:
	 * <+0>:	sub    $0x8,%rsp
	 * <+4>:	mov    0x30(%rsp),%rax
   	 * <+9>:	mov    0x38(%rsp),%r10
	 * 
	 * So when optimizing with O2, you need 9 instead 8 bytes to not break anything
	 */
	orig_eleven_args = Framework::Hooking::hookFunc(reinterpret_cast<void*>(eleven_args), reinterpret_cast<void*>(eleven_args_hook), 9);
	assert(orig_eleven_args != nullptr);
	printf("Hooked eleven_args with eleven_args_hook\n");
#endif

#ifdef ENABLE_HOOKING_PTRSWAP
	// We have to be very careful so we don't create 2 seperate call instructions
	for(int i = 0; i <= 1; i++) {
		assert(hookMe(1,2,3) == i == 0 ? 6 : 12);
		if(i == 0) {
			orig_hookMe = Framework::Hooking::relativePtrSwap(static_cast<char*>(hookMe_retaddr) - (FRAMEWORK_NEAR_JMP_LENGTH - 1), reinterpret_cast<void*>(hook));
			assert(orig_hookMe != nullptr);
		} else if(i == 1) {
			printf("Hooked hookMe with hook using a ptr swap\n");
		}
	}
#endif

#if defined(ENABLE_RETURN_ADDRESS) || defined(ENABLE_HOOKING_DETOUR)
	testStruct.a = 1;
	testStruct.b = 2;
	testStruct.c = 3;
#endif

#ifdef ENABLE_RETURN_ADDRESS
	// Testing whether or not you can reuse these invokers (I hate this code, but for tests, it is good enough)
	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int>(reinterpret_cast<void*>(no_args))) == 1337);
	}
	printf("no_args test passed\n");

	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, test_struct>(reinterpret_cast<void*>(one_arg), testStruct)) == 1337);
	}
	printf("one_arg test passed\n");

	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, short, int>(reinterpret_cast<void*>(two_args), 1, 2)) == 1337);
	}
	printf("two_args test passed\n");

	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, short, int, long>(reinterpret_cast<void*>(three_args), 1, 2, 3)) == 1337);
	}
	printf("three_args test passed\n");

	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, short, int, long, unsigned int>(reinterpret_cast<void*>(four_args), 1, 2, 3, 4)) == 1337);
	}
	printf("four_args test passed\n");

	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int>(reinterpret_cast<void*>(five_args), 1, 2, 3, 4, 5)) == 1337);
	}
	printf("five_args test passed\n");

	// Testing different levels of deepness in the call stack
	test();
	
	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int, long, unsigned int, long long int>(reinterpret_cast<void*>(ten_args), 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)) == 1337);
	}
	printf("ten_args test passed\n");

#ifdef ENABLE_HOOKING_DETOUR
	for(int i = 0; i <= 2; i++) {
		assert(eleven_args(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, testStruct) == 1338);
	}
	printf("Verified that eleven_args is hooked with a detour\n");
#else
	for(int i = 0; i <= 2; i++) {
		assert((Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int, long, unsigned int, long long int, test_struct>(reinterpret_cast<void*>(ten_args), 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, testStruct)) == 1337);
	}
#endif
	printf("eleven_args test passed\n");
#elif defined(ENABLE_HOOKING_DETOUR)
	assert(eleven_args(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, testStruct) == 1338);
	printf("Verified that eleven_args is hooked with a detour\n");
#endif
	printf("---------------------------------------------\n");
	printf("All tests passed\n");
	printf("---------------------------------------------\n");
}
