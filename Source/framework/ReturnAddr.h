//
// Created by johannes on 4/17/22.
//

#ifndef FRAMEWORK_RETURNADDR_H
#define FRAMEWORK_RETURNADDR_H

#include "PatternScan.h"
#include "Memory.h"
#include "Assembly.h"

namespace Framework {
	namespace ReturnAddr {
		/*
		 * The return address spoofer expects this to be set
		 * This has to be a byte-sequence which contains the following:
		 * c9	leave
		 * c3	ret
		 */
		static void *ret_instruction_addr = nullptr;
		
		static Pattern leave_ret_instruction(
			"\xC9\xC3",
			"xx"
		);

		namespace {
			Pattern call_instruction(
				"\xFF\x00\x00\x00\x00\x00\x90",
				"x?????x"
			);
			
			__attribute((noinline)) void *rip() {
				return __builtin_extract_return_addr(__builtin_return_address(0));
			}
		}
		
		template<typename Ret, typename... Args>
		static __attribute((noinline, optimize("O0"))) auto invoke(void *method, Args... args) -> Ret {
			static bool mutated = false;
			
			if (!mutated) {
				void *instruction = rip();
				
				void *base = PatternScan::searchPattern(call_instruction, instruction);
				
				int callRegisterOffset = 1;
				
				// call r8-r15 instructions have a 0x41 modifier
				if (*(static_cast<unsigned char *>(base) - 1) == 0x41) {
					base = static_cast<char *>(base) - 1;
					callRegisterOffset++;
				}
				
				// search the first nop instruction
				int length = callRegisterOffset + 1 /* Register is one byte */;
				while (*(static_cast<unsigned char *>(base) + length) != 0x90)
					length++;
				
				Memory::protect(base, PROT_READ | PROT_WRITE | PROT_EXEC);
				
				// move real code back
				memcpy(static_cast<char *>(base) + 12, base, length);
				
				Assembly::writeAbsPush(base, static_cast<char *>(ret_instruction_addr));
				
				*(static_cast<char *>(base) + FRAMEWORK_ABS_PUSH_LENGTH + callRegisterOffset) += 0x10; // convert call to jmp instruction
				
				Memory::protect(base, PROT_READ | PROT_EXEC);
				mutated = true;
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
			// Indirect return by callee, carrying the return value of method
		}
	}
}
#endif //FRAMEWORK_RETURNADDR_H
