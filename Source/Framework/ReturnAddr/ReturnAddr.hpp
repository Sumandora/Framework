#ifndef FRAMEWORK_RETURNADDR_H
#define FRAMEWORK_RETURNADDR_H

#include "../PatternScan/PatternScan.hpp"
#include "../Memory/Memory.hpp"
#include "../Assembly/Assembly.hpp"

#include <mutex>
#include <sys/mman.h>
#include <cstring>

namespace Framework {
	namespace ReturnAddr {
#ifdef FRAMEWORK_ENABLE_RETURN_ADDRESS
// #ifdef FRAMEWORK_ENABLE_PATTERN_SCANNING
		// // A pattern, which matches the data, which ret_instruction_addr should point at
		// static Pattern leave_ret_instruction(
			// "\xC9\xC3",
			// "xx"
		// );
// #endif

		namespace {
#ifdef FRAMEWORK_ENABLE_PATTERN_SCANNING
			// This pattern is used internally, to find the location of the jump instruction
			// Pattern call_instruction(
				// "\xFF\x00\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90",
				// "x?????xxxxxxxx"
			// );
#endif

			// This is pretty hacky, but it works
			__attribute((noinline)) void* rip() {
				return __builtin_extract_return_addr(__builtin_return_address(0));
			}
		}

		/*
		 * Explanation for ret_instruction_addr:
		 * The return address spoofer expects this to be set
		 * This has to be a byte-sequence which contains the following:
		 * c9	leave
		 * c3	ret
		 */
		template<typename Ret, typename... Args>
		static __attribute((noinline, optimize("O0"))) auto invoke(void* method, const void* ret_instruction_addr, Args... args) -> Ret {
			static bool mutated = false;
			static std::mutex mutex;

			while (!mutated) { // Only being run once (in theory; multithreading), for the ability to break anytime
				if(!ret_instruction_addr) {
					// The user hasn't set it yet ._.
					// Alert his debugger if he has one attached
					__asm ("int3");
				}
				mutex.lock();
				if(mutated) {
					/*
					 * This is a special case, where one thread called this, started the mutation
					 * Another thread called this function and the mutation wasn't done
					 * The mutex will prevent anymore weird action by the second call, but
					 * we still have to move this thread out of this while loop.
					 * So this weird if-statement has to be here, this is also the reason
					 * why the mutated-variable is being set to true before unlocking the mutex
					 * If someone has a better/cleaner solution to this, please contribute it
					 */
					break;
				}
				void* instruction = rip();


// #ifdef FRAMEWORK_ENABLE_PATTERN_SCANNING
				// void* base = call_instruction.searchPattern(instruction);
// #else
				void* base = instruction;
				while(
					*static_cast<unsigned char*>(base) != 0xFF ||
					*(static_cast<unsigned char*>(base) + 6) != 0x90 ||
					*(static_cast<unsigned char*>(base) + 7) != 0x90 ||
					*(static_cast<unsigned char*>(base) + 8) != 0x90 ||
					*(static_cast<unsigned char*>(base) + 9) != 0x90 ||
					*(static_cast<unsigned char*>(base) + 10) != 0x90 ||
					*(static_cast<unsigned char*>(base) + 11) != 0x90 ||
					*(static_cast<unsigned char*>(base) + 12) != 0x90 ||
					*(static_cast<unsigned char*>(base) + 13) != 0x90
				) {
					base = static_cast<char*>(base) + 1;
				}
// #endif
				
				int callRegisterOffset = 1;
				
				// call r8-r15 instructions have a 0x41 modifier
				if (*(static_cast<unsigned char*>(base) - 1) == 0x41) {
					base = static_cast<char*>(base) - 1;
					callRegisterOffset++;
				}
				
				// search the first nop instruction
				int length = callRegisterOffset + 1 /* Register is one byte */;
				while (*(static_cast<unsigned char*>(base) + length) != 0x90)
					length++;
				
				Memory::protect(base, PROT_READ | PROT_WRITE | PROT_EXEC);
				
				// If the method is split into 2 pages then we have to ensure that both of them are writable
				Memory::protect(static_cast<char*>(base) + FRAMEWORK_ABS_PUSH_LENGTH + length, PROT_READ | PROT_WRITE | PROT_EXEC);
				
				// move real code back; This no longer uses memcpy, because it lead to segfaults, which have a reason, which is beyond me.
				// memcpy(static_cast<char*>(base) + FRAMEWORK_ABS_PUSH_LENGTH, base, length);
				int i = 0;
				while (i < length) {
					*(static_cast<char*>(base) + FRAMEWORK_ABS_PUSH_LENGTH + i) = *(static_cast<char*>(base) + i);
					i++;
				}
				
				Assembly::writeAbsPush(base, static_cast<const char*>(ret_instruction_addr));
				
				// convert call to jmp instruction
				*(static_cast<char*>(base) + FRAMEWORK_ABS_PUSH_LENGTH + callRegisterOffset) += 0x10;

				Memory::protect(base, PROT_READ | PROT_EXEC);
				Memory::protect(static_cast<char*>(base) + FRAMEWORK_ABS_PUSH_LENGTH + length, PROT_READ | PROT_EXEC);
				
				mutated = true;
				mutex.unlock(); // Allow other threads to continue
				break;
			}
			
			// This call will later be substituted by push+jmp instructions 
			reinterpret_cast<Ret(*)(Args...)>(method)(args...);
			// we need 12 more bytes for the push instruction
			__asm(
				"nop;"
				"nop;"
				"nop;"
				"nop;"
				"nop;"
				"nop;"
				"nop;"
				"nop;"
				"nop;"
				"nop;"
				"nop;"
				"nop;" // This one seems to be useless, because gcc generates a nop by itself, but keep it for safety reasons
			);
			// Indirect return, carrying the return value of method
		}
#endif

	}
}
#endif

