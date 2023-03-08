#include "Assembly.hpp"

#include <string.h>

#ifdef FRAMEWORK_ENABLE_HOOKING_DETOUR
void Framework::Assembly::writeNearJmp(void* addr, const void* goal) {
	unsigned char jmpInstruction[] = {
		0xE9, 0x0, 0x0, 0x0, 0x0 // jmp goal
	};
	// Calculation for a relative jmp
	void* jmpTarget = reinterpret_cast<void*>(static_cast<const char*>(goal) -
		(static_cast<char*>(addr) +
		FRAMEWORK_NEAR_JMP_LENGTH)); // Jumps always start at the rip, which has already increased
	memcpy(jmpInstruction + 1, &jmpTarget, sizeof(jmpInstruction) - 1 /* E9 */);
	memcpy(addr, jmpInstruction, sizeof(jmpInstruction));
}
#endif

/*
 * --OUTDATED--
 * The following 2 methods both use r15 as register
 * It appears, that the System V ABI does not explicitly define a use for r12-r15 + a few others
 * So gcc does not use it extensively
 * r15 seems to be our best bet when it comes to a mostly unused register
 * But it still is a gamble
 * Making a third parameter, which specifies the register would be a solution, but some
 * instructions change their encoding, because of addressing attributes
 * e.g. jmp/call instructions get longer when using certain registers (including r15)
 * So we might end up wasting a byte, which would be a pretty porely designed code
 * --OUTDATED--
 * 
 * NEW EXPLAINATION:
 * It is correct, that r15 is not used often, but the context we use these both functions in
 * makes rax a way better bet
 * The only problem:
 * When hooking mid-function this could overwrite the contents of rax.
 * Hooking mid is considered unsafe even when using r15, so this is not
 * a issue for me
 */
#ifdef FRAMEWORK_ENABLE_RETURN_ADDRESS
void Framework::Assembly::writeAbsPush(void* addr, const void* value) {
	unsigned char absPushInstruction[] = {
		0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov rax, value
		0x50 //push rax
	};
	memcpy(absPushInstruction + 2, &value, sizeof(void*));
	memcpy(addr, absPushInstruction, FRAMEWORK_ABS_PUSH_LENGTH);
}
#endif

#if defined(FRAMEWORK_ENABLE_HOOKING_DETOUR) || defined(FRAMEWORK_ENABLE_HOOKING_PTRSWAP)
void Framework::Assembly::writeAbsJmp(void* addr, const void* goal) {
	unsigned char absJumpInstructions[] = {
		0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov rax, goal
		0xFF, 0xE0 //jmp rax
	};
	memcpy(absJumpInstructions + 2, &goal, sizeof(void*));
	memcpy(addr, absJumpInstructions, FRAMEWORK_ABS_JMP_LENGTH);
}
#endif
