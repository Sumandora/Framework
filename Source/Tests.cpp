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

struct test_struct {
	int a;
	int b;
	long long int c;
};

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
	assert(__builtin_return_address(0) == Framework::ReturnAddr::ret_instruction_addr);
	return 1337;
}

void* orig_eleven_args;

int eleven_args_hook(short a, int b, long c, unsigned int d, long long int e, short f, int g, long h, unsigned int i,
				long long int j, test_struct k) {
	printf("Hello, eleven_args is hooked ^^\n");
	return Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int, long, unsigned int, long long int, test_struct>(reinterpret_cast<void*>(orig_eleven_args), a, b, c, d, e, f, g, h, i, j, k);
}

void test2() {

	int retVal = Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int, long>(reinterpret_cast<void*>(eight_args), 1, 2, 3, 4, 5, 6, 7, 8);
	assert(retVal == 1337);
	printf("eight_args test passed\n");

	retVal = Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int, long, unsigned int>(reinterpret_cast<void*>(nine_args), 1, 2, 3, 4, 5, 6, 7, 8, 9);
	assert(retVal == 1337);
	printf("nine_args test passed\n");
}

void test() {
	test_struct testStruct{};
	testStruct.a = 1;
	testStruct.b = 2;
	testStruct.c = 3;

	int retVal = Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, test_struct>(reinterpret_cast<void*>(six_args), 1, 2, 3, 4, 5, testStruct);
	assert(retVal == 1337);
	printf("six_args test passed\n");

	retVal = Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int>(reinterpret_cast<void*>(seven_args), 1, 2, 3, 4, 5, 6, 7);
	assert(retVal == 1337);
	printf("seven_args test passed\n");

	test2();

}
/*
 * This method has to be kept at O0, because if anything else is used it will crash if you try to reuse the invokers from the Return Address Spoofer
 */
int __attribute((optimize("O0"))) main() {
	test_struct testStruct{};
	testStruct.a = 1;
	testStruct.b = 2;
	testStruct.c = 3;

	printf("Starting tests\n");

	Framework::ReturnAddr::ret_instruction_addr = Framework::PatternScan::searchPattern(Framework::ReturnAddr::leave_ret_instruction, reinterpret_cast<void*>(main));
	assert(Framework::ReturnAddr::ret_instruction_addr != nullptr);
	printf("Found ret_instruction at %p\n", Framework::ReturnAddr::ret_instruction_addr);

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
	
	// Testing whether or not you can reuse these invokers
	for(int i = 0; i <= 2; i++) {
		assert(Framework::ReturnAddr::invoke<int>(reinterpret_cast<void*>(no_args)) == 1337);
		printf("no_args test passed\n");

		int retVal = Framework::ReturnAddr::invoke<int, test_struct>(reinterpret_cast<void*>(one_arg), testStruct);
		assert(retVal == 1337);
		printf("one_arg test passed\n");

		retVal = Framework::ReturnAddr::invoke<int, short, int>(reinterpret_cast<void*>(two_args), 1, 2);
		assert(retVal == 1337);
		printf("two_args test passed\n");

		retVal = Framework::ReturnAddr::invoke<int, short, int, long>(reinterpret_cast<void*>(three_args), 1, 2, 3);
		assert(retVal == 1337);
		printf("three_args test passed\n");

		retVal = Framework::ReturnAddr::invoke<int, short, int, long, unsigned int>(reinterpret_cast<void*>(four_args), 1, 2, 3, 4);
		assert(retVal == 1337);
		printf("four_args test passed\n");

		retVal = Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int>(reinterpret_cast<void*>(five_args), 1, 2, 3, 4, 5);
		assert(retVal == 1337);
		printf("five_args test passed\n");
	
		// Testing different levels of deepness in the call stack
		test();

		retVal = Framework::ReturnAddr::invoke<int, short, int, long, unsigned int, long long int, short, int, long, unsigned int, long long int>(reinterpret_cast<void*>(ten_args), 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
		assert(retVal == 1337);
		printf("ten_args test passed\n");

		assert(eleven_args(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, testStruct) == 1337);
		printf("eleven_args test passed\n");

		printf("---------------------------------------------\n");
		printf("All tests passed\n");
		printf("---------------------------------------------\n");
	}
}
